const gridSize = 16;

const grid = document.getElementById("grid");
const controls = document.getElementById("controls");
let selectedColor = "";

const applyColor = (cell) => {
    switch (selectedColor) {
        case "ERASER":
            cell.style.backgroundColor = "";

        default:
            cell.style.backgroundColor = selectedColor;
    }
};

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
    0: "#000000",
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

for (let i = 0; i < 24; i++) {
    const colorCell = document.createElement("div");
    colorCell.classList.add("color-cell");
    colorCell.id = `color-${i}`;

    colorCell.style.backgroundColor = colors[i];
    colorCell.addEventListener("mousedown", () => {
        selectedColor = `${colors[i]}`;
        console.log(selectedColor);
    });

    controls.appendChild(colorCell);
}

/* Botones Controles*/

const eraser = document.getElementById("eraser");
const clear = document.getElementById("clear");
const colorPicker = document.getElementById("color-picker");

const clearScreen = () => {
    grid.childNodes.forEach((cell) => {
        cell.style.backgroundColor = "";
    });
};

eraser.addEventListener("mousedown", () => {
    selectedColor = "ERASER";
});

clear.addEventListener("mousedown", clearScreen);

colorPicker.addEventListener("input", (e) => {
    selectedColor = e.target.value;
    console.log(selectedColor);
});

/* Save grid */

function saveGridData() {
    const gridData = [];
    let k = 0;

    for (i = 0; i < gridSize; i++) {
        for (j = 0; j < gridSize; j++) {
            let color = grid.childNodes[k].style.backgroundColor;

            if (color) {
                gridData.push([[i, j], color]);
            }

            k++;
        }
    }

    return JSON.stringify(gridData);
}

const saveButton = document.getElementById("save-btn");

saveButton.addEventListener("click", () => {
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

    // Clean up
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
});

/* Load grid */

function loadGridData(gridDataJSON) {
    const gridData = JSON.parse(gridDataJSON);
    console.log(gridData);

    clearScreen();

    gridData.forEach((cell) => {
        let pos = cell[0][0] * gridSize + cell[0][1];
        grid.childNodes[pos].style.backgroundColor = cell[1];
    });
}

function loadGridDataFromFile(file) {
    const reader = new FileReader();

    reader.onload = (event) => {
        const gridDataJSON = event.target.result;
        loadGridData(gridDataJSON);
    };

    reader.readAsText(file);
}

const loadInput = document.getElementById("load-input");

loadInput.addEventListener("change", (event) => {
    const file = event.target.files[0];
    if (file) {
        loadGridDataFromFile(file);
    }
});
