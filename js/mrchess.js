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
                Board.checkForSpecialMove(fromPos, toPos);
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
            var toPos = $(this).parent().attr("id");
            // TODO: There must be a better way to lookup the old position
            var fromPos;
            $("div#selected").each(function(key, value) {
                fromPos = $(this).parent().parent().attr("id");
            });
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
            // If this was a special move (castling or promotion), complete it
            Board.checkForSpecialMove(fromPos, toPos);
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
    // TODO: This needs a lot of work. For one, only call it when we know it's a special move.
    checkForSpecialMove: function(fromPos, toPos) {
        // Castling
        var isSpecialMove = false;
        var rookFrom = "";
        var rookTo = "";
        if (fromPos == "e1") {
            if (toPos == "c1" && positions["c1"].indexOf("king") >= 0) {
                rookFrom = "a1";
                rookTo = "d1";
                isSpecialMove = true;
            }
            else if (toPos == "g1" && positions["g1"].indexOf("king") >= 0) {
                rookFrom = "h1";
                rookTo = "f1";
                isSpecialMove = true;
            }
        }
        else if (fromPos == "e8") {
            if (toPos == "c8" && positions["c8"].indexOf("king") >= 0) {
                rookFrom = "a8";
                rookTo = "d8";
                isSpecialMove = true;
            }
            else if (toPos == "g8" && positions["g8"].indexOf("king") >= 0) {
                rookFrom = "h8";
                rookTo = "f8";
                isSpecialMove = true;
            }
        }
        if (isSpecialMove == true) {
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

    var urlParams = new URLSearchParams(window.location.search); // Get all URL parameters
    if (urlParams.get("debug")) {
        Debug.init();
    }

    // If human player is not white, play the first move
    if (Game.humanPlayer != "white") {
        Computer.playMove();
    }
});

