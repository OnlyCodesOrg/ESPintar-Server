const ws = new WebSocket("ws://" + window.location.hostname + ":81/");

const gridSize = 16;

const root = document.documentElement;
const grid = document.getElementById("grid");
let video;
let canvas;
let canvasContext;

function onOpenCvReady() {
    video = document.getElementById("camera");
    canvas = document.createElement("canvas");
    canvasContext = canvas.getContext("2d");
    canvas.width = gridSize;
    canvas.height = gridSize;

    // Initialize Camera
    navigator.mediaDevices
        .getUserMedia({ video: true, audio: false })
        .then((stream) => {
            video.srcObject = stream;
        })
        .catch((error) => {
            console.log(error);
        });

    let FPS = 15;
    video.addEventListener("canplay", function () {
        setInterval(processCamera, 1000 / FPS);
    });
}

// Actualizar grid
function processCamera() {
    canvasContext.drawImage(video, 0, 0, gridSize, gridSize);
    const imageData = canvasContext.getImageData(0, 0, gridSize, gridSize);
    const data = imageData.data;
    const gridData = [];

    for (let i = 0; i < gridSize; i++) {
        for (let j = 0; j < gridSize; j++) {
            const pixel = (j * gridSize + i) * 4;

            const red = data[pixel];
            const green = data[pixel + 1];
            const blue = data[pixel + 2];

            const cell = document.getElementById(`cell-${i + j * gridSize}`);
            cell.style.backgroundColor = `rgb(${red}, ${green}, ${blue})`;

            const pixelData = [
                [i, j],
                [red, green, blue],
            ];
            gridData.push(pixelData);
        }
    }
    // ws.send(JSON.stringify(gridData));
}

const createGrid = (() => {
    for (let i = 0; i < gridSize * gridSize; i++) {
        // Create cells
        const cell = document.createElement("div");
        cell.classList.add("grid-cell");
        cell.id = `cell-${i}`;
        grid.appendChild(cell);

        // Desktop drawing
        cell.addEventListener("mousedown", () => applyColor(cell));
        cell.addEventListener("mouseover", (e) => {
            if (e.buttons === 1) applyColor(cell); // Check hover and mouse click
        });
    }
})();
