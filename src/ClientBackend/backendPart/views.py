import json
import socket
import os
import subprocess
from django.http import HttpResponse
from django.shortcuts import render
from django.template import loader
from django.contrib.auth.models import User
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth import authenticate, login, logout
from django.core.exceptions import ObjectDoesNotExist
from . import models, forms

## Path to user's AI files
AIDBPath = "../../AIDataBase/"

@csrf_exempt
## Handler of request of index page
# @param request - user request
def index(request):
    print("Index view started")

    userList = []
    for user in User.objects.all():
        userList.append(user)

    profileList = []
    for profile in models.Profile.objects.all():
        profileList.append(profile)

    context = {
        #'userList': userList,
        #'profileList': profileList,
        'result': "",
    }

    template = loader.get_template('backendPart/index.html')

    return HttpResponse(template.render(context, request))

## Read game json from file
# Method for reading game server's json from file
# @param fileName = name of json's file
def readDirectlyFromFile(fileName):
    filePath = 'backendPart/static/backendPart/'
    file = open(filePath + fileName)
    data = json.load(file)
    return data

## Read game json from socket
# Method for reading game server's json from socket
# @param blockSize - size of blocks to read per request
def readFromSocket(blockSize):
    port = 8000
    data = b""
    # Try to open socket
    try:
        sock = socket.socket()  # Do not know when file closes to know then to close socket, so now open and close it in every request
        sock.connect(('localhost', port))
    except ConnectionRefusedError:
        return False, data
    else:  # if socket opened
        sock.setblocking(0)
        # Try to read from socket
        try:
            tmp = sock.recv(blockSize)
            while tmp:
                data += tmp
                tmp = sock.recv(blockSize)
        except socket.error:
            return False, data
        sock.close()


@csrf_exempt
## Get game map json
# @param request - user request
def getGameMapJson(request):
    fileName = 'game_map .json'
    # Getting JSON with TCP from game server
    blockSize = 1024  # Get JSON partly
    readed, data = readFromSocket(blockSize)
    if not readed:
        print("read directly")
        data = readDirectlyFromFile(fileName)
    return HttpResponse(json.dumps(data), content_type='application/json')


@csrf_exempt
## Get game objects json
# @param request - user request
def getObjectsJson(request):
    fileName = 'objects.json'
    # Getting JSON with TCP from game server
    blockSize = 128  # Get JSON partly
    readed, data = readFromSocket(blockSize)
    if not readed:
        data = readDirectlyFromFile(fileName)
    return HttpResponse(json.dumps(data), content_type='application/json')

## Handler of request of registration page
# @param request - user request
def registration(request):
    template = loader.get_template('backendPart/registration.html')
    return HttpResponse(template.render({}, request))


@csrf_exempt
## Handler of request to register the user
# @param request - user request
def registerUser(request):
    print("start registering view")
    result = ""
    if request.method == 'POST':
        form = forms.SignUpForm(request.POST)
        if form.is_valid():
            username = form.cleaned_data['username']
            password = form.cleaned_data['password']
            email = form.cleaned_data['email']

            print(username, password, email)
            try:
                user = User.objects.get(username=username)
            except ObjectDoesNotExist:
                user = None

            if user is not None:
                print("Unsuccessful registration: such username exist! Please choose other username.")
                result = "Unsuccessful registration: such user exist! Please choose other username."
            else:
                newUser = User.objects.create_user(username, email, password)
                newUser.save()

                # create user directory
                path = AIDBPath + username
                os.makedirs(path)
                # add profile in database
                person = models.Profile()
                person.user = newUser
                person.aiFolderPath = path
                person.save()

                print("\nSign up was successful!")
                result = "Sign up was successful!"
        else:
            print("Input data is not valid")
            result = "Input data is not valid"
    else:
        print("request is not POST")
    return render(request, 'backendPart/registration.html', {'result': result})


@csrf_exempt
## Handler of request to log in the user
# @param request - user request
def logIn(request):
    # template = loader.get_template('backendPart/index.html')
    print("log in view started")
    result = ""
    if request.method == 'POST':
        form = forms.logInForm(request.POST)
        if form.is_valid():
            username = form.cleaned_data['username']
            password = form.cleaned_data['password']
            user = authenticate(request, username=username, password=password)

            if user is not None:
                print("Have such user")
                login(request, user)
            else:
                print("Username or password is not valid")
                result = "Username or password is not valid"
        else:
            print("Input data is not valid")
            result = "Username or password is not valid"
    else:
        print("request is not POST")
    return render(request, 'backendPart/index.html', {'result': result})


@csrf_exempt
## Handler of request to log out the user
# @param request - user request
def logOut(request):
    print("log out view started")
    logout(request)
    return render(request, 'backendPart/index.html', {'result': ""})


@csrf_exempt
## Handler of request to upload user files (AI files)
# @param request - user request
# permitted file's extensions: .cpp, .c, .h
def uploadFile(request):
    permittedExtensions = ["cpp", "h", "c"]
    permittedFileSize = 1e5

    result = "Files were uploaded" # Default result
    if request.method == 'POST':
        print("upload view started")
        form = forms.UploadFileForm(request.POST, request.FILES)
        if form.is_valid():
            # Get game name
            gameName = request.POST['gameName']

            # Create game path for user
            filesPath = AIDBPath + "/" + request.user.username + "/" + gameName + "/"
            if os.path.exists(filesPath):
                existingFilesList = [f for f in os.listdir(filesPath)]
                for f in existingFilesList:
                    os.remove(os.path.join(filesPath, f))  # delete existing files
            else:
                os.makedirs(filesPath)

            # Iterating throw uploaded files
            uploadedfilesList = request.FILES.getlist('FileName')
            for file in uploadedfilesList:
                fileName = file.name
                # Check file extensions (permitted only .cpp, .h) and size
                extension = fileName.split(".")[-1]
                if extension not in permittedExtensions:
                    result = "Not permitted extension: " + fileName
                    break
                if file.size > permittedFileSize:
                    result = "Not permitted file size: " + fileName
                    break

                # Write sent file
                with open(filesPath + fileName, 'wb+') as destination:
                    for chunk in file.chunks():
                        destination.write(chunk)
            # Static check AI files
            # ...
            return render(request, 'backendPart/games/game' + gameName + '.html', {'result': result})
    return render(request, 'backendPart/gameGallery.html', {})

## Handler of request of game gallery page
# @param request - user request
def gameGallery(request):
    template = loader.get_template('backendPart/gameGallery.html')
    return HttpResponse(template.render({}, request))

## Handler of request of game PickItUp page
# @param request - user request
def gamePickItUp(request):
    template = loader.get_template('backendPart/games/gamePickItUp.html')
    return HttpResponse(template.render({}, request))

## Handler of request of game MakeItYours page
# @param request - user request
def gameMakeItYours(request):
    template = loader.get_template('backendPart/games/gameMakeItYours.html')
    return HttpResponse(template.render({}, request))

## Handler of request of game BattleTanks page
# @param request - user request
def gameBattleTanks(request):
    template = loader.get_template('backendPart/games/gameBattleTanks.html')
    return HttpResponse(template.render({}, request))


@csrf_exempt
## Handler of request to start game
# @param request - user request
def startGame(request):
    if request.method == 'POST':
        form = forms.StartGameForm(request.POST)
        if form.is_valid():
            gameName = form.cleaned_data['gameName']
            # start game with name = gameName
            pathToBuilds = '../../builds/'
            subprocess.Popen(
                pathToBuilds + 'x64/Debug/BattleServer.exe -p ..\\..\\src\\ClientBackend\\backendPart\\static\\backendPart\\')
            subprocess.Popen(pathToBuilds + 'x64/Debug/AI_1.exe')
            subprocess.Popen(pathToBuilds + 'x64/Debug/AI_2.exe')
            return render(request, 'backendPart/gameGallery.html', {})

# Handler of request to get user guide
# @param request - user request
def getUserGuide(request):
    filename = '..\\..\\documents\\user_guide.docx'
    data = open(filename, "rb").read()
    print(data)
    response = HttpResponse(data, content_type='application/vnd.openxmlformats-officedocument.wordprocessingml.document')
    response['Content-Length'] = os.path.getsize(filename)
    return response

