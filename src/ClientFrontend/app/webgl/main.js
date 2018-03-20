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

class parser {
    parser() {
        this.objXml = new XMLHttpRequest();
        this.objXml.open("GET", "./game_map .json", false);
        this.objXml.send(null);

        this.squares = JSON.parse(this.objXml.responseText);

        this.objXml = new XMLHttpRequest();
        this.objXml.open("GET", "./objects.json", false);
        this.objXml.send(null);

        this.objects = JSON.parse(this.objXml.responseText);
    }
}

class Graphics2d {

//
// Start here
//
    constructor() {
        this.canvas = document.querySelector('#glcanvas');
        this.window_width_aspect = this.canvas.width / window.innerWidth;
        this.window_height_aspect = this.canvas.height / window.innerHeight;
        this.gl = this.canvas.getContext('webgl');
        this.vsSource = `
            attribute vec4 aVertexPosition;
        
            uniform mat4 uModelViewMatrix;
            uniform mat4 uProjectionMatrix;
        
            void main() {
              gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;
            }
          `;

        // Fragment shader program

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
                color: this.gl.getUniformLocation(this.shaderProgram, 'color')
            },
        };

        this.buffers = this.initBuffers(this.gl);
    }

    main() {

        // Runs each time the DOM window resize event fires.
        // Resets the canvas dimensions to match window,
        // then draws the new borders accordingly.

        // If we don't have a GL context, give up now

        if (!this.gl) {
            alert('Unable to initialize WebGL. Your browser or machine may not support it.');
            return;
        }
        this.drawScene(this.gl, this.programInfo, this.buffers);
    }

    resizeDisplay() {
        this.canvas.width = window.innerWidth * this.window_width_aspect;
        this.canvas.height = window.innerHeight * this.window_height_aspect;
        this.drawScene(this.gl, this.programInfo, this.buffers);
        return;
    }

//
// initBuffers
//
// Initialize the buffers we'll need. For this demo, we just
// have one object -- a simple two-dimensional square.
//

    initBuffers(gl) {

        // Create a buffer for the square's positions.

        const positionBuffer = gl.createBuffer();

        // Select the positionBuffer as the one to apply buffer
        // operations to from here out.

        gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

        // Now create an array of positions for the square.

        this.positions = [
            0.1, 0.1,
            -0.1, 0.1,
            0.1, -0.1,
            -0.1, -0.1,
        ];


        // Now pass the list of positions into WebGL to build the
        // shape. We do this by creating a Float32Array from the
        // JavaScript array, then use it to fill the current buffer.

        gl.bufferData(gl.ARRAY_BUFFER,
            new Float32Array(this.positions),
            gl.STATIC_DRAW);

        return {
            position: positionBuffer,
        };
    }

//
// Draw the scene.
//

    drawScene(gl, programInfo, buffers) {
        this.parser = new parser();
        this.parser.parser();
        this.squares = this.parser.squares;
        this.objects = this.parser.objects;
        gl.clearColor(this.squares.colours.background[0],
            this.squares.colours.background[1],
            this.squares.colours.background[2], 1.0);  // Clear to black, fully opaque
        gl.clearDepth(1.0);                 // Clear everything
        gl.enable(gl.DEPTH_TEST);           // Enable depth testing
        gl.depthFunc(gl.LEQUAL);            // Near things obscure far things

        gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
        // Clear the canvas before we start drawing on it.

        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        // Create a perspective matrix, a special matrix that is
        // used to simulate the distortion of perspective in a camera.
        // Our field of view is 45 degrees, with a height/height
        // ratio that matches the display size of the canvas
        // and we only want to see objects between 0.1 units
        // and 100 units away from the camera.

        const fieldOfView = 45 * Math.PI / 180 * (gl.canvas.clientWidth / gl.canvas.clientHeight);   // in radians
        let aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
        const zNear = 0.1;
        const zFar = 100.0;

        this.projectionMatrix = mat4.create();
        // Set the drawing position to the "identity" point, which is
        // the center of the scene.
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
        // note: glmatrix.js always has the first argument
        // as the destination to receive the result.
        mat4.perspective(this.projectionMatrix,
            fieldOfView,
            aspect,
            zNear,
            zFar);



        // Now move the drawing position a bit to where we want to
        // start drawing the square.

        mat4.translate(this.modelViewMatrix,     // destination matrix
            this. modelViewMatrix,     // matrix to translate
            [-1.0, -1.0, -5]);  // amount to translate
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
                mat4.translate(this.modelViewMatrix,     // destination matrix
                    this.modelViewMatrix, [-1.0 + this.addX, -1.0 + this.addY, -5.0]);


                // Tell WebGL how to pull out the positions from the position
                // buffer into the vertexPosition attribute.
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

                // Tell WebGL to use our program when drawing

                gl.useProgram(programInfo.program);

                // Set the shader uniforms
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
        return;
    }

//
// Initialize a shader program, so WebGL knows how to draw our data
//

    initShaderProgram(gl, vsSource, fsSource) {
        const vertexShader = this.loadShader(gl, gl.VERTEX_SHADER, vsSource);
        const fragmentShader = this.loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

        // Create the shader program

        const shaderProgram = gl.createProgram();
        gl.attachShader(shaderProgram, vertexShader);
        gl.attachShader(shaderProgram, fragmentShader);
        gl.linkProgram(shaderProgram);

        // If creating the shader program failed, alert

        if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
            alert('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
            return null;
        }

        return shaderProgram;
    }

//
// creates a shader of the given type, uploads the source and
// compiles it.
//

    loadShader(gl, type, source) {
        const shader = gl.createShader(type);

        // Send the source to the shader object

        gl.shaderSource(shader, source);

        // Compile the shader program

        gl.compileShader(shader);

        // See if it compiled successfully

        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            alert('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
            gl.deleteShader(shader);
            return null;
        }

        return shader;
    }
}


let graphics = new Graphics2d();


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


