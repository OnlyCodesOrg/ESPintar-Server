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

for (let i = 0; i < 16 * 16; i++) {
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

eraser.addEventListener("mousedown", () => {
    selectedColor = "ERASER";
});

clear.addEventListener("mousedown", () => {
    grid.childNodes.forEach((cell) => {
        cell.style.backgroundColor = "";
    });
});

colorPicker.addEventListener("input", (e) => {
    selectedColor = e.target.value;
    console.log(selectedColor);
});
