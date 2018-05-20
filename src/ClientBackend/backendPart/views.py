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

AIDBPath = "../../AIDataBase"


@csrf_exempt
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


def readDirectlyFromFile(fileName):
    filePath = 'backendPart/static/backendPart/'
    file = open(filePath + fileName)
    data = json.load(file)
    return data


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
def getObjectsJson(request):
    fileName = 'objects.json'
    # Getting JSON with TCP from game server
    blockSize = 128  # Get JSON partly
    readed, data = readFromSocket(blockSize)
    if not readed:
        data = readDirectlyFromFile(fileName)
    return HttpResponse(json.dumps(data), content_type='application/json')


def registration(request):
    template = loader.get_template('backendPart/registration.html')
    return HttpResponse(template.render({}, request))


@csrf_exempt
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
def logOut(request):
    print("log out view started")
    logout(request)
    return render(request, 'backendPart/index.html', {'result': ""})


@csrf_exempt
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


def gameGallery(request):
    template = loader.get_template('backendPart/gameGallery.html')
    return HttpResponse(template.render({}, request))

@csrf_exempt
def gamePickItUp(request):
    template = loader.get_template('backendPart/games/gamePickItUp.html')
    return HttpResponse(template.render({}, request))

@csrf_exempt
def gameMakeItYours(request):
    template = loader.get_template('backendPart/games/gameMakeItYours.html')
    return HttpResponse(template.render({}, request))

@csrf_exempt
def game2(request):
    template = loader.get_template('backendPart/games/game2.html')
    return HttpResponse(template.render({}, request))


@csrf_exempt
def startGame(request):
    if request.method == 'POST':
        form = forms.StartGameForm(request.POST)
        if form.is_valid():
            gameName = form.cleaned_data['gameName']
            print("excelent" + gameName)
            # start game with name = gameName

            return render(request, 'backendPart/gameGallery.html', {})

