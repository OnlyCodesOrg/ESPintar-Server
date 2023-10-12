const grid = document.getElementById("grid");

for (let i = 0; i < 16 * 16; i++) {
    const cell = document.createElement("div");
    cell.classList.add("grid-item");
    grid.appendChild(cell);
}
