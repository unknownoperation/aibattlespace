class Graphics2d {

    constructor() {
        // получаем ссылку на canvas с текстом
        this.textCanvas = document.getElementById("text");
        this.ctx = this.textCanvas.getContext("2d");
        this.canvas = document.querySelector('#glcanvas');
        this.gl = this.canvas.getContext('webgl');
        this.window_width_aspect = this.canvas.width / window.innerWidth;
        this.window_height_aspect = this.canvas.height / window.innerHeight;
        this.vsSource = `
            attribute vec4 aVertexPosition;
            uniform mat4 uModelViewMatrix;
            uniform mat4 uProjectionMatrix;
        
            void main() {
              gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;
            }
          `;

        this.fsSource = `
            precision highp float;  
            uniform vec3 color;
            void main() {
                   gl_FragColor = vec4(color.r,color.g,color.b, 1.0);
            }
          `;

        this.shaderProgram = this.initShaderProgram(this.gl, this.vsSource, this.fsSource);

        this.programInfo = {
            program: this.shaderProgram,
            attribLocations: {
                vertexPosition: this.gl.getAttribLocation(this.shaderProgram, 'aVertexPosition'),
            },
            uniformLocations: {
                projectionMatrix: this.gl.getUniformLocation(this.shaderProgram, 'uProjectionMatrix'),
                modelViewMatrix: this.gl.getUniformLocation(this.shaderProgram, 'uModelViewMatrix'),
                color: this.gl.getUniformLocation(this.shaderProgram, 'color'),
            },
        };

        this.buffers = this.initBuffers(this.gl);
        this.objXml = new XMLHttpRequest();
        this.objXml.open("GET", "game_map .json", false);
        this.objXml.send(null);
        this.squares = JSON.parse(this.objXml.responseText);
    }

    main() {
        if (!this.gl) {
            alert('Unable to initialize WebGL. Your browser or machine may not support it.');
            return;
        }
        this.drawScene(this.gl, this.programInfo, this.buffers);
    }

    resizeDisplay() {
        this.canvas.width = window.innerWidth * this.window_width_aspect;
        this.canvas.height = window.innerHeight * this.window_height_aspect;
		this.canvas.width = window.innerWidth * this.window_width_aspect;
		if(this.canvas.width < 300)
			this.canvas.width = 300;
		if(this.canvas.width > 1024)
			this.canvas.width = 1024;
        this.canvas.height = window.innerHeight * this.window_height_aspect;
		if(this.canvas.height<300)
			this.canvas.height = 300;
		if(this.canvas.height > 640)
			this.canvas.height = 640;
        this.drawScene(this.gl, this.programInfo, this.buffers);
        this.drawScene(this.gl, this.programInfo, this.buffers);
        return;
    }

    initBuffers(gl) {

        const positionBuffer = gl.createBuffer();

        gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

        this.positions = [
            0.1, 0.1,
            -0.1, 0.1,
            0.1, -0.1,
            -0.1, -0.1,
        ];
                                                                
        gl.bufferData(gl.ARRAY_BUFFER,
            new Float32Array(this.positions),
            gl.STATIC_DRAW);

        return {
            position: positionBuffer,
        };
    }
                                                      
    drawScene(gl, programInfo, buffers) {
        this.parser = new parser();
        this.parser.parser();
        this.objects = this.parser.objects;
        gl.clearColor(this.squares.colours.background[0],
            this.squares.colours.background[1],
            this.squares.colours.background[2], 1.0); 
        gl.clearDepth(1.0);             
        gl.enable(gl.DEPTH_TEST);       
        gl.depthFunc(gl.LEQUAL);            
        gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        this.ctx.clearRect(0, 0, this.ctx.canvas.width, this.ctx.canvas.height);

        const fieldOfView = 45 * Math.PI / 180 * (gl.canvas.clientHeight / gl.canvas.clientWidth); 
        let aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
        const zNear = 0.1;
        const zFar = 100.0;

        this.projectionMatrix = mat4.create();
        this.modelViewMatrix = mat4.create();
        this.matrix_of_squares = [];

        for (let i = 0; i < this.squares.field.height; i++)
            this.matrix_of_squares[i] = [];

        for (let i = 0; i < this.squares.field.height; i++)
            for (let j = 0; j < this.squares.field.width; j++)
                this.matrix_of_squares[i][j] = new cell();

        for (let i = 0; i < this.squares.obstruction.length; i++) {
            this.matrix_of_squares[this.squares.obstruction[i].position[0]][this.squares.obstruction[i].position[1]]
                .set_cell(this.squares.obstruction[i].type, this.squares.colours.barrier,
                    this.squares.obstruction[i].position[0], this.squares.obstruction[i].position[1]);
        }
        for (let i = 0; i < this.squares.field.height; i++)
            for (let j = 0; j < this.squares.field.width; j++)
                if (!this.matrix_of_squares[i][j].type)
                    this.matrix_of_squares[i][j].set_cell("freespace", this.squares.colours.freespace, i, j);

        for (let i = 0; i < this.objects.chips.length; i++) {
            this.matrix_of_squares[this.objects.chips[i].position[0]][this.objects.chips[i].position[1]]
                .set_cell("chips",this.squares.colours.chips,this.objects.chips[i].position[0],this.objects.chips[i].position[1]);
        }

        this.matrix_of_squares[this.objects.players[0].position[0]][this.objects.players[0].position[1]]
            .set_cell("player_1",this.squares.colours.player_1,
                this.objects.players[0].position[0],this.objects.players[0].position[1]);

        this.matrix_of_squares[this.objects.players[1].position[0]][this.objects.players[1].position[1]]
            .set_cell("player_2",this.squares.colours.player_2,
                this.objects.players[1].position[0],this.objects.players[1].position[1]);
        mat4.perspective(this.projectionMatrix,
            fieldOfView,
            aspect,
            zNear,
            zFar);
        mat4.translate(this.modelViewMatrix,   
            this. modelViewMatrix,     
            [-1.0, -1.0, -5]);  
        for (let i = 0; i < this.squares.field.height; i++) {
            for (let j = 0; j < this.squares.field.width; j++) {
                if (j != 0)
                    this.addX = 2 / this.squares.field.width * (j);
                else
                    this.addX = 0;
                if (i != 0)
                    this.addY = 2 / this.squares.field.height * (i);
                else
                    this.addY = 0;
                this.modelViewMatrix = mat4.create();
                mat4.translate(this.modelViewMatrix,     
                    this.modelViewMatrix, [-1.0 + this.addX, -1.0 + this.addY, -5.0]);
                {
                    const numComponents = 2;
                    const type = gl.FLOAT;
                    const normalize = false;
                    const stride = 0;
                    const offset = 0;
                    gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
                    gl.vertexAttribPointer(
                        programInfo.attribLocations.vertexPosition,
                        numComponents,
                        type,
                        normalize,
                        stride,
                        offset);
                    gl.enableVertexAttribArray(
                        programInfo.attribLocations.vertexPosition);
                }
                gl.useProgram(programInfo.program);
                gl.uniform3fv(programInfo.uniformLocations.color, this.matrix_of_squares[j][i].color);
                gl.uniformMatrix4fv(
                    programInfo.uniformLocations.projectionMatrix,
                    false,
                    this.projectionMatrix);
                gl.uniformMatrix4fv(
                    programInfo.uniformLocations.modelViewMatrix,
                    false,
                    this.modelViewMatrix);
                {
                    const offset = 0;
                    const vertexCount = 4;
                    gl.drawArrays(gl.TRIANGLE_STRIP, offset, vertexCount);
                }
            }
        }
        //this.objects.game_stage = "compliting";
        switch(this.objects.game_stage) {
            case "running": {
                this.ctx.fillText("state: " + this.objects.game_stage, 300, 40);
                this.ctx.fillText("player " + this.objects.players[0].ID, 50, 350);
                this.ctx.fillText("points " + this.objects.players[0].points, 50, 360);
                this.ctx.fillText("player " + this.objects.players[1].ID, 550, 350);
                this.ctx.fillText("points " + this.objects.players[1].points, 550, 360);
                break;
            }
            case "compliting":
            {
                this.ctx.fillStyle = "rgba(127,127,127,0.5)";
                this.ctx.fillRect(0, 0, this.canvas.clientWidth, this.canvas.clientHeight);
                this.ctx.fillStyle = "rgba(255,0,0,1)";
                for(let i = 0; i < this.objects.winner.length;i++)
                    this.ctx.fillText("Player " + this.objects.winner[0] + " wins", 260, 240+10*i);
                break;
            }
        }
        return;
    }

    initShaderProgram(gl, vsSource, fsSource) {
        const vertexShader = this.loadShader(gl, gl.VERTEX_SHADER, vsSource);
        const fragmentShader = this.loadShader(gl, gl.FRAGMENT_SHADER, fsSource);
        const shaderProgram = gl.createProgram();

        gl.attachShader(shaderProgram, vertexShader);
        gl.attachShader(shaderProgram, fragmentShader);
        gl.linkProgram(shaderProgram);

        if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
            alert('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
            return null;
        }

        return shaderProgram;
    }

    loadShader(gl, type, source) {
        const shader = gl.createShader(type);

        gl.shaderSource(shader, source);  
        gl.compileShader(shader);

        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            alert('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
            gl.deleteShader(shader);
            return null;
        }

        return shader;
    }
}
