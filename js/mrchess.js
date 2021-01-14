var board = {
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

$(document).ready(function() {
  drawBoard();
});

var drawBoard = function() {
  for (var position in board) {
    console.log(position);
    $("table#board td#" + position).html("<a href=\"#\" class=\"" + board[position] + "\"></a>");
  }
}