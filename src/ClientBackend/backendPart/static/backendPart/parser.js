
class parser {

    parser() {
        this.objXml = new XMLHttpRequest();
        this.objXml.open("GET", "game_map .json", false);
        this.objXml.send(null);
        this.squares = JSON.parse(this.objXml.responseText);
        this.objXml = new XMLHttpRequest();
        this.objXml.open("GET", "objects.json", false);
        this.objXml.send(null);
        this.objects = JSON.parse(this.objXml.responseText);
    }
}
