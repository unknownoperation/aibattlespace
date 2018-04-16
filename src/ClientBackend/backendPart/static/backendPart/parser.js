
class parser {

    parser() {
        this.objects = null;
        try {//Did I ever tell you what the definition of insanity is?
            this.objXml = new XMLHttpRequest();
            this.objXml.open("GET", "objects.json", false);
            this.objXml.send(null);
            this.objects = JSON.parse(this.objXml.responseText);
        }catch(e){
            while (1) {//Insanity is doing the exact… over and over again, expecting… shit to change.
                this.objXml = new XMLHttpRequest();
                this.objXml.open("GET", "objects.json", false);
                this.objXml.send(null);
                if(this.objXml.status != 500)
                    break;
            }
            this.objects = JSON.parse(this.objXml.responseText);
        }
    }
}
