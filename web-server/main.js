const ws = new WebSocket("ws://" + window.location.hostname + ":81/");

/* Conexión con ESP32 */
const initWebsocket = (() => {
    ws.onmessage = (evt) => {
        evtObj = json.parse(evt.data);
        console.log(evtObj);

        loadGridData(evtObj);
    };
})();

const gridSize = 16;

const root = document.documentElement;
const grid = document.getElementById("grid");
const controls = document.getElementById("controls");
let selectedColor = "";

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

const applyColor = (cell) => {
    const cellID = parseInt(cell.id.match(/\d+/)[0]);
    const i = cellID % gridSize;
    const j = Math.floor(cellID / gridSize);
    let data;

    switch (selectedColor) {
        case "ERASER":
            cell.style.backgroundColor = "";

        default:
            cell.style.backgroundColor = selectedColor;
    }

    if (selectedColor == "ERASER") {
        data = [
            [i, j],
            [0, 0, 0],
        ];
    }

    if (cell.style.backgroundColor) {
        const cellColor = cell.style.backgroundColor;
        let rgbValues = cellColor.slice(4, -1);
        let rgbArray = rgbValues.split(", ").map(Number);

        var red = rgbArray[0];
        var green = rgbArray[1];
        var blue = rgbArray[2];

        data = [
            [i, j],
            [red, green, blue],
        ];
    }

    ws.send(JSON.stringify(data));
    console.log(JSON.stringify(data));
};

// Mobile drawing
const draw = (touch) => {
    const cell = document.elementFromPoint(touch.clientX, touch.clientY);
    if (cell.classList.contains("grid-cell")) {
        applyColor(cell);
    }
};

grid.addEventListener("touchstart", (e) => {
    isDrawing = true;
    draw(e.touches[0]);
});

grid.addEventListener("touchmove", (e) => {
    if (e.touches.length >= 2) return; // Allow zoom
    e.preventDefault();
    if (isDrawing) draw(e.touches[0]);
});

grid.addEventListener("touchend", () => {
    isDrawing = false;
});

/* Color Selector */

const colors = {
    0: "#ffffff",
    1: "#232035",
    2: "#44293c",
    3: "#ff0000",
    4: "#df7027",
    5: "#d8a067",
    6: "#efc39b",
    7: "#fbf337",
    8: "#98e450",
    9: "#6abf31",
    10: "#37946e",
    11: "#4b682e",
    12: "#524b24",
    13: "#323c39",
    14: "#3e3f75",
    15: "#316083",
    16: "#5a6ee0",
    17: "#639afe",
    18: "#5ecde5",
    19: "#cadafc",
    20: "#dddddd",
    21: "#9badb7",
    22: "#857f87",
    23: "#686a6b",
};

const createColorsGrid = (() => {
    for (let i = 0; i < 24; i++) {
        const colorCell = document.createElement("div");
        colorCell.classList.add("color-cell");
        colorCell.id = `color-${i}`;

        colorCell.style.backgroundColor = colors[i];
        colorCell.addEventListener("mousedown", () => {
            selectedColor = `${colors[i]}`;
            root.style.setProperty("--selected-color", `${colors[i]}`);
        });

        controls.appendChild(colorCell);
    }
})();

/* Botones Controles*/

const eraser = document.getElementById("eraser");
const clear = document.getElementById("clear");
const colorPicker = document.getElementById("color-picker");

const clearScreen = () => {
    grid.childNodes.forEach((cell) => {
        cell.style.backgroundColor = "";
    });

    const data = JSON.stringify("LIMPIAR");
    ws.send(data);
    console.log(data);
};

eraser.addEventListener("mousedown", () => {
    selectedColor = "ERASER";
    root.style.setProperty("--selected-color", "#000000");
});

clear.addEventListener("mousedown", clearScreen);

colorPicker.addEventListener("input", (e) => {
    selectedColor = e.target.value;
    root.style.setProperty("--selected-color", selectedColor);
});

/* Save grid */

const saveGridData = () => {
    const gridData = [];
    let i;
    let j;
    let k = 0;

    for (i = 0; i < gridSize; i++) {
        for (j = 0; j < gridSize; j++) {
            let color = grid.childNodes[k].style.backgroundColor;

            if (color && color != "rgb(0, 0, 0)") {
                let rgbValues = color.slice(4, -1);
                let rgbArray = rgbValues.split(", ").map(Number);

                var red = rgbArray[0];
                var green = rgbArray[1];
                var blue = rgbArray[2];

                const data = [
                    [i, j],
                    [red, green, blue],
                ];
                gridData.push(data);
            }

            k++;
        }
    }

    return JSON.stringify(gridData);
};

const saveGridJSON = () => {
    const gridDataJSON = saveGridData();
    const blob = new Blob([gridDataJSON], { type: "application/json" });
    const url = URL.createObjectURL(blob);

    // Create a link to download the file
    const a = document.createElement("a");
    a.href = url;
    a.download = "gridData.json";
    a.style.display = "none";
    document.body.appendChild(a);

    // Trigger a click event to download the file
    a.click();

    document.body.removeChild(a);
    URL.revokeObjectURL(url);
};

const saveButton = document.getElementById("save-btn");
saveButton.addEventListener("click", saveGridJSON);

/* Load grid */

const loadGridData = (gridDataJSON) => {
    const gridData = JSON.parse(gridDataJSON);
    console.log(gridData);

    clearScreen();

    gridData.forEach((cell) => {
        let pos = cell[0][0] * gridSize + cell[0][1];
        grid.childNodes[
            pos
        ].style.backgroundColor = `rgb(${cell[1][0]}, ${cell[1][1]}, ${cell[1][2]})`;
    });
};

const loadGridDataFromFile = (file) => {
    const reader = new FileReader();

    reader.onload = (event) => {
        const gridDataJSON = event.target.result;
        loadGridData(gridDataJSON);
        ws.send(gridDataJSON);
    };

    reader.readAsText(file);
};

const loadInput = document.getElementById("load-input");
loadInput.addEventListener("change", (event) => {
    const file = event.target.files[0];
    if (file) {
        loadGridDataFromFile(file);
    }
});
