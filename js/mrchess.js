var positions = {
    "A8": "black castle",
    "B8": "black knight",
    "C8": "black bishop",
    "D8": "black queen",
    "E8": "black king",
    "F8": "black bishop",
    "G8": "black knight",
    "H8": "black castle",
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
    "A1": "white castle",
    "B1": "white knight",
    "C1": "white bishop",
    "D1": "white queen",
    "E1": "white king",
    "F1": "white bishop",
    "G1": "white knight",
    "H1": "white castle"
}

var Game = {
    humanPlayer: "white",
    turn: "white"
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
                    action: "getAvailableMoves",
                    board: positions,
                    piece: $(this).parent().attr("id")
                }),
                dataType: "json",
                type: "POST",
                url: "cgi-bin/mrchess.cgi",
                success: function(result) {
                    // Set available moves
                    $("a.availableMove").remove();
                    $.each(result.availableMoves, function(index, position) {
                        $("td#" + position).append("<a class=\"availableMove\"></a>");
                    });
                    Board.setActions();
                },
                error: function (xhr, ajaxOptions, thrownError) {
                    console.log(xhr.status);
                    console.log(thrownError);
                  }
            });
        });
        Board.setActions();
    },
    setActions: function() {
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
            Game.turn = (Game.humanPlayer == "white") ? "black" : "white";
            // Update HTML board
            $("table#board td#" + old_position + " a.piece").remove();
            $("table#board td#" + new_position).html("<a class=\"human piece " + positions[new_position] + "\"></a>");
            $("div#selected").remove()
            $("a.availableMove").remove();
            Board.setActions();
        })
    }
}

$(document).ready(function() {
    Board.init();
});

