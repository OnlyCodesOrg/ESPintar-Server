const grid = document.getElementById("grid");
const controls = document.getElementById("controls");

for (let i = 0; i < 16 * 16; i++) {
    // Create cells
    const cell = document.createElement("div");
    cell.classList.add("grid-cell");
    cell.id = `cell-${i}`;

    // Desktop drawing
    cell.addEventListener("mousedown", (e) => {
        cell.style.backgroundColor = selectedColor;
    });

    cell.addEventListener("mouseover", (e) => {
        if (e.buttons === 1) {
            cell.style.backgroundColor = selectedColor;
        }
    });

    cell.addEventListener("touchstart", () => {
        cell.style.backgroundColor = selectedColor;
    });

    cell.addEventListener("touchmove", () => {
        cell.style.backgroundColor = selectedColor;
    });

    // Append cell
    grid.appendChild(cell);
}

// Mobile drawing
function draw(touch) {
    const cell = document.elementFromPoint(touch.clientX, touch.clientY);

    if (cell && cell.classList.contains("grid-cell")) {
        cell.style.backgroundColor = selectedColor;
    }
}

grid.addEventListener("touchstart", (e) => {
    isDrawing = true;
    draw(e.touches[0]);
});

grid.addEventListener("touchmove", (e) => {
    e.preventDefault(); // Prevent scroll to refresh
    if (isDrawing) {
        draw(e.touches[0]);
    }
});

grid.addEventListener("touchend", () => {
    isDrawing = false;
});

/* Color Selector */

const colors = {
    0: "#000000",
    1: "#232035",
    2: "#44293c",
    3: "#673930",
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
