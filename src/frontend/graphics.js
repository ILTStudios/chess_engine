let board = [];

let white_castle_qs = false;
let white_castle_ks = false;
let black_castle_qs = false;
let black_castle_ks = false;

let rank = 1;
// display checkerboard
for(let square = 1; square < 65; square += 1){
    if(rank % 2){
        let box = document.createElement("div");
        box.className = (square % 2) ? "square_w" : "square_b";
        box.id = square;
        document.getElementById("board").appendChild(box)
    }else{
        let box = document.createElement("div");
        box.className = (square % 2) ? "square_b" : "square_w";
        box.id = square;
        document.getElementById("board").appendChild(box)
    }
    if(square % 8 === 0){
        rank += 1;
    }
}

// display FEN string
const isNumeric = (string) => /^[+-]?\d+(\.\d+)?$/.test(string)
const FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
let square_index = 0;
// loop over FEN string
FEN.split("").forEach((element, index) => {

    // if numeric, then 
    if(isNumeric(element)){
        for(let i = 0; i < Number(element); i += 1){
            square_index += 1;
        }
    }
    else if(element == "/"){
        
    }
    else if(element != "/" && !isNumeric(element)){
        let image = document.createElement("img");
        image.src = `./src/${element === element.toUpperCase() ? "w" : "b"}${element.toLowerCase()}.png`;        
        document.getElementById(`${square_index + 1}`).appendChild(image);
        image.draggable = true;
        square_index += 1;
    }
});

// drag and drop type shit
const boardElement = document.getElementById("board");
let draggedPiece = null;
// When you start a drag
boardElement.addEventListener("dragstart", (event) => {
    if (event.target.tagName === "IMG") draggedPiece = event.target;
});
// let em drop
boardElement.addEventListener("dragover", (event) => {
    event.preventDefault();
});
// when dropped
boardElement.addEventListener("drop", (event) => {
    event.preventDefault();
    let targetSquare = event.target.closest("#board > div");
    if (!targetSquare || !draggedPiece) return;
    // remove piece if there
    let existingPiece = targetSquare.querySelector("img");
    if (existingPiece) existingPiece.remove();
    // move to destination
    targetSquare.appendChild(draggedPiece);
    draggedPiece = null;
});