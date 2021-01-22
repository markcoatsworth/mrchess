var positions = {
    "A8": "black rook",
    "B8": "black knight",
    "C8": "black bishop",
    "D8": "black queen",
    "E8": "black king",
    "F8": "black bishop",
    "G8": "black knight",
    "H8": "black rook",
    "A7": "black pawn",
    "B7": "black pawn",
    "C7": "black pawn",
    "D7": "black pawn",
    "E7": "black pawn",
    "F7": "black pawn",
    "G7": "black pawn",
    "H7": "black pawn",
    "A2": "white pawn",
    "B2": "white pawn",
    "C2": "white pawn",
    "D2": "white pawn",
    "E2": "white pawn",
    "F2": "white pawn",
    "G2": "white pawn",
    "H2": "white pawn",
    "A1": "white rook",
    "B1": "white knight",
    "C1": "white bishop",
    "D1": "white queen",
    "E1": "white king",
    "F1": "white bishop",
    "G1": "white knight",
    "H1": "white rook"
}

var Game = {
    humanPlayer: "white",
    turn: "white"
}

var Debug = {
    init: function() {
        $("div#debug-panel").show();
        $("div#game-panel").addClass("split");
        $("div#debug-panel").addClass("split");
    },
    log: function(msg) {
        console.log(msg);
        $("div#debug-log").append(JSON.stringify(msg) + "<br/>--<br/>");
    }
}

var MRCHESS = {
    playMove: function() {
        // Call our backend CGI script to get computer move
        $.ajax({
            contentType: "application/json; charset=utf-8",
            data: JSON.stringify({
                action: "getMove",
                board: positions,
                color: Game.turn,
                debugger: $("#debugger-wait").get(0).checked
            }),
            dataType: "json",
            type: "POST",
            url: "cgi-bin/mrchess.cgi",
            success: function(result) {
                Debug.log(result);
                var fromPos = result.move.from;
                var toPos = result.move.to;
                var piece = positions[fromPos];
                // Update data structures
                delete positions[fromPos];
                positions[toPos] = piece;
                Game.turn = (Game.turn == "white") ? "black" : "white";
                // Update HTML board
                $("table#board td#" + fromPos + " a.piece").remove();
                $("table#board td#" + toPos).html("<a class=\"piece " + positions[toPos] + "\"></a>");
                $("div#selected").remove()
                $("a.availableMove").remove();
                Board.setActions();
                // Update status
                $("div#status-bar").html("Black moved " + fromPos + " to " + toPos + ". White turn");
            },
            error: function (xhr, ajaxOptions, thrownError) {
                Debug.log(xhr.status);
                Debug.log(thrownError);
            }
        });
    }
}

var Board = {
    init: function() {
        for (var position in positions) {
            if (positions[position].indexOf(Game.humanPlayer) >= 0) {
                $("table#board td#" + position).html("<a class=\"human piece " + positions[position] + "\"></a>");
            }
            else {
                $("table#board td#" + position).html("<a class=\"piece " + positions[position] + "\"></a>");
            }
        }
        Board.setActions();
    },
    setActions: function() {
        // Unbind all previous a.click event
        $("a.piece").off("click");
        // Now set new events
        $("a.piece").click(function() {
            // Only allow clicks on human player pieces
            if (!($(this).hasClass("human"))) {
                return;
            }
            // Set the selection
            $("div#selected").remove();
            $(this).append("<div id=\"selected\"></div>");
            // Call our backend CGI script to get available moves
            $.ajax({
                contentType: "application/json; charset=utf-8",
                data: JSON.stringify({
                    action: "getPieceAvailableMoves",
                    board: positions,
                    debugger: $("#debugger-wait").get(0).checked,
                    position: $(this).parent().attr("id")
                }),
                dataType: "json",
                type: "POST",
                url: "cgi-bin/mrchess.cgi",
                success: function(results) {
                    Debug.log(results);
                    // Set available moves
                    $("a.availableMove").remove();
                    $.each(results.moves, function(index, position) {
                        $("td#" + position).append("<a class=\"availableMove\"></a>");
                    });
                    Board.setActions();
                },
                error: function (xhr, ajaxOptions, thrownError) {
                    Debug.log(xhr.status);
                    Debug.log(thrownError);
                }
            });
        });
        // We don't need to unbind previous a.availableMoves because they don't exist,
        // all previous a.availableMove elements get removed as part of the click event
        $("a.availableMove").click(function() {
            var new_position = $(this).parent().attr("id");
            // TODO: There must be a better way to lookup the old position
            var old_position;
            $("div#selected").each(function(key, value) {
                old_position = $(this).parent().parent().attr("id");
            });
            var piece = positions[old_position];
            // Update data structures
            delete positions[old_position];
            positions[new_position] = piece;
            Game.turn = (Game.turn == "white") ? "black" : "white";
            // Update HTML board
            $("table#board td#" + old_position + " a.piece").remove();
            $("table#board td#" + new_position).html("<a class=\"human piece " + positions[new_position] + "\"></a>");
            $("div#selected").remove()
            $("a.availableMove").remove();
            Board.setActions();
            // Update status and trigger next move
            $("div#status-bar").html("Black turn. Computer is thinking...");
            MRCHESS.playMove();
        })
    }
}

$(document).ready(function() {
    Board.init();

    var urlParams = new URLSearchParams(window.location.search); // Get all URL parameters
    if (urlParams.get("debug")) {
        Debug.init();
    }
});

