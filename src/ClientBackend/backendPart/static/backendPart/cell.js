
class cell {
    constructor() {
        this.type = null;
        this.color = null;
        this.positionX = null;
        this.positionY = null;
    }

    set_cell(type, color, positionX, positionY) {
        this.type = type;
        this.color = color;
        this.positionX = positionX;
        this.positionY = positionY;
    }
}

