                         
var graphics = new Graphics2d();

function main(gameName) {
    startGame(gameName);
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

function reinitStartJson() {

}

function startGame(gameName) {
    var req = new XMLHttpRequest();
    var formData = new FormData();
    formData.append("gameName", gameName);
    req.open("POST", "startGame");
    req.send(formData);
}
