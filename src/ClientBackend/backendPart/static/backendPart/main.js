                         
var graphics = new Graphics2d();

function main() {
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

function Complete() {
    var Elem= "Name: " + document.Send_Data.Name.value +
	"\nFile: " + document.Send_Data.FileName.value;
    alert(Elem);
}


