                         
var graphics = new Graphics2d();

function main() {
    graphics.main();
    renderloop();
    window.addEventListener('resize', resizeCanvas, false);
}

function renderloop() {
    graphics.main();
    window.setTimeout(this.renderloop, 1);
}

function resizeCanvas() {
    graphics.resizeDisplay();
}


