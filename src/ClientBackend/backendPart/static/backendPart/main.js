                         
var graphics = new Graphics2d();

function main() {
    alert("ti loh");
    graphics.main();
    renderloop();
    window.addEventListener('resize', resizeCanvas, false);
}

function renderloop() {
    graphics.main();
    window.setTimeout(this.renderloop, 1000/60);  
}

function resizeCanvas() {
    graphics.resizeDisplay();
}


