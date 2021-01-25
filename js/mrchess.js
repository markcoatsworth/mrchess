var positions = {
    "a8": "black rook",
    "b8": "black knight",
    "c8": "black bishop",
    "d8": "black queen",
    "e8": "black king",
    "f8": "black bishop",
    "g8": "black knight",
    "h8": "black rook",
    "a7": "black pawn",
    "b7": "black pawn",
    "c7": "black pawn",
    "d7": "black pawn",
    "e7": "black pawn",
    "f7": "black pawn",
    "g7": "black pawn",
    "h7": "black pawn",
    "a2": "white pawn",
    "b2": "white pawn",
    "c2": "white pawn",
    "d2": "white pawn",
    "e2": "white pawn",
    "f2": "white pawn",
    "g2": "white pawn",
    "h2": "white pawn",
    "a1": "white rook",
    "b1": "white knight",
    "c1": "white bishop",
    "d1": "white queen",
    "e1": "white king",
    "f1": "white bishop",
    "g1": "white knight",
    "h1": "white rook"
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

var Computer = {
    playMove: function() {
        var postData = JSON.stringify({
            action: "getMove",
            board: positions,
            color: Game.turn,
            debugger: $("#debugger-wait").get(0).checked
        });
        //Debug.log("Sending postData = " + postData);
        // Call our backend CGI script to get computer move
        $.ajax({
            contentType: "application/json; charset=utf-8",
            data: postData,
            dataType: "json",
            type: "POST",
            url: "cgi-bin/mrchess.cgi",
            success: function(result) {
                //Debug.log(result);
                var fromPos = result.move.substring(0, 2);
                var toPos = result.move.substring(2, 4);
                var fromPiece = positions[fromPos];
                var toPiece = positions[toPos];
                // Update data structures
                delete positions[fromPos];
                positions[toPos] = fromPiece;
                Game.turn = (Game.turn == "white") ? "black" : "white";
                computerPlayer = (Game.humanPlayer == "white" ) ? "black" : "white";
                // Update HTML board
                $("table#board td#" + fromPos + " a.piece").remove();
                $("table#board td#" + toPos).html("<a class=\"piece " + positions[toPos] + "\"></a>");
                $("div#selected").remove()
                $("a.availableMove").remove();
                // If this was a special move (castling or promotion), complete it
                if (result.move.length > 4) {
                    Board.playSpecialMove(result.move);
                }
                // Check if game is over, and update status
                if (toPiece != undefined && toPiece.indexOf("king") >= 0) {
                    $("div#status-bar").html("Game over. <span class=\"capitalize\">" + computerPlayer + "</span> wins! <a href=\"javascript:location.reload();\">New game</a>");
                    Board.removeActions();
                }
                else {
                    $("div#status-bar").html("<span class=\"capitalize\">" + computerPlayer + " </span> moved " + fromPos + " to " + toPos + ". <span class=\"capitalize\">" + Game.turn + "</span> turn");
                    Board.setActions();
                }
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
    removeActions: function() {
        $("a.piece").off("click");
    },
    setActions: function() {
        // Remove previous actions
        Board.removeActions();
        // Now set new events
        $("a.piece").click(function() {
            // Only allow clicks on human player pieces
            if (!($(this).hasClass("human"))) {
                return;
            }
            // Set the selection
            $("div#selected").remove();
            $(this).append("<div id=\"selected\"></div>");
            var postData = JSON.stringify({
                action: "getPieceAvailableMoves",
                board: positions,
                debugger: $("#debugger-wait").get(0).checked,
                position: $(this).parent().attr("id")
            });
            //Debug.log("Sending postData = " + postData);
            // Call our backend CGI script to get available moves
            $.ajax({
                contentType: "application/json; charset=utf-8",
                data: postData,
                dataType: "json",
                type: "POST",
                url: "cgi-bin/mrchess.cgi",
                success: function(results) {
                    Debug.log(results);
                    // Set available moves
                    $("a.availableMove").remove();
                    $.each(results.moves, function(index, move) {
                        var toPos = move.substring(2, 4);
                        $("td#" + toPos).append("<a id=\"" + move + "\" class=\"availableMove\"></a>");
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
            var move = $(this).attr("id")
            var fromPos = move.substring(0, 2);
            var toPos = move.substring(2, 4);
            var fromPiece = positions[fromPos];
            var toPiece = positions[toPos];
            // Update data structures
            delete positions[fromPos];
            positions[toPos] = fromPiece;
            Game.turn = (Game.turn == "white") ? "black" : "white";
            // Update HTML board
            $("table#board td#" + fromPos + " a.piece").remove();
            $("table#board td#" + toPos).html("<a class=\"human piece " + positions[toPos] + "\"></a>");
            $("div#selected").remove()
            $("a.availableMove").remove();
            Debug.log("Checking for special move, move.length = " + move.length);
            // If this was a special move (castling or promotion), complete it
            if (move.length > 4) {
                Board.playSpecialMove(move);
            }
            // Check if game is over, and update status
            if (toPiece != undefined && toPiece.indexOf("king") >= 0) {
                $("div#status-bar").html("Game over. <span class=\"capitalize\">" + Game.humanPlayer + "</span> wins! <a href=\"javascript:location.reload();\">New game</a>");
                Board.removeActions();
            }
            else {
                $("div#status-bar").html("<span class=\"capitalize\">" + Game.turn + "</span> turn. Computer is thinking...");
                Board.setActions();
                Computer.playMove();
            }
        })
    },
    playSpecialMove: function(move) {
        // We can assume that move is a 5-character string representing a valid special move:
        // * First two characters represent the move-from position
        // * Next two characters represent the move-to position
        // * Last character represents the type of special move (c = castling, q = queen promotion, k = knight promotion)
        if (move.charAt(4) == "c") {
            var rookFrom;
            var rookTo;
            switch (move) {
                case "e1c1c":
                    rookFrom = "a1";
                    rookTo = "d1";
                    break;
                case "e1g1c":
                    rookFrom = "h1";
                    rookTo = "f1";
                    break;
                case "e8c8c":
                    rookFrom = "a8";
                    rookTo = "d8";
                    break;
                case "e8g8c":
                    rookFrom = "h8";
                    rookTo = "f8";
                    break;
            }
            Debug.log("Moving rookFrom = " + rookFrom + ", rookTo = " + rookTo);
            $("table#board td#" + rookFrom + " a.piece").remove();
            $("table#board td#" + rookTo).html("<a class=\"human piece " + positions[rookFrom] + "\"></a>");
            var rookPiece = positions[rookFrom];
            delete positions[rookFrom];
            positions[rookTo] = rookPiece;
        }
    }
}

$(document).ready(function() {
    Board.init();

    // If the url contains a "debug" variable, turn on the debug panel
    var urlParams = new URLSearchParams(window.location.search);
    if (urlParams.get("debug")) {
        Debug.init();
    }

    // If human player is not white, play the first move
    if (Game.humanPlayer != "white") {
        Computer.playMove();
    }
});

