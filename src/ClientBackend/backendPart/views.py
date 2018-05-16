import json
import socket
# Create your views here.
from django.http import HttpResponse
from django.shortcuts import render
from django.template import loader
from django.contrib.auth.models import User
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth import authenticate, login, logout
from django.core.exceptions import ObjectDoesNotExist
from . import models, forms

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
        'userList': userList,
        'profileList': profileList,
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
    context = {}
    template = loader.get_template('backendPart/registration.html')
    return HttpResponse(template.render(context, request))


@csrf_exempt
def registerUser(request):
    print("start registering view")
    context = {}
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
                context = {
                    'result': "Unsuccessful registration: such user exist! Please choose other username.",
                }
            else:
                newUser = User.objects.create_user(username, email, password)
                newUser.save()

                person = models.Profile()
                person.user = newUser
                person.aiFolderPath = ""
                person.save()

                print("\nSign up was successful!")
                context = {
                    'result': "\nSign up was successful!",
                }
        else:
            print("Input data is not valid")
            context = {
                'result': "Input data is not valid",
            }
    else:
        print("request is not POST")
        context = {
            'result': "",
        }
    return render(request, 'backendPart/registration.html', context)


@csrf_exempt
def logIn(request):
    #template = loader.get_template('backendPart/index.html')
    print("log in view started")
    if request.method == 'POST':
        form = forms.logInForm(request.POST)
        print(form.is_valid())
        print(form.errors)
        if form.is_valid():
            username = form.cleaned_data['username']
            password = form.cleaned_data['password']
            user = authenticate(request, username=username, password=password)

            if user is not None:
                print("Have such user")
                login(request, user)
                context = {
                    'result': "",
                }
            else:
                print("Username or password is not valid")
                context = {
                    'result': "Username or password is not valid",
                }
        else:
            print("Input data is not valid")
            context = {
                'result': "Username or password is not valid",
            }
    else:
        print("request is not POST")
        context = {
            'result': "",
        }
    return render(request, 'backendPart/index.html', context)

@csrf_exempt
def logOut(request):
    print("log out view started")
    logout(request)
    context = {
        'result': "",
    }
    return render(request, 'backendPart/index.html', context)

@csrf_exempt
def uploadFile(request):
    context = {}
    if request.method == 'POST':
        print("upload view started")
        form = forms.UploadFileForm(request.POST, request.FILES)
        print(request.FILES)
        print(request.FILES['FileName'])
        files = request.FILES['FileName']
        print(files)
        if form.is_valid():
            path = ""
            fileInd = 1
            fileList = request.FILES.getlist('FileName')
            for file in fileList:
                fileName = file.name
                #expansion = fileName.split(".")[-1]
                # Write sent file
                with open(str(path) + fileName, 'wb+') as destination:
                    for chunk in file.chunks():
                        destination.write(chunk)
                fileInd += 1
        context = {
            'result': "Files were uploaded",
        }
    return render(request, 'backendPart/index.html', context)

def gameGallery(request):
    context = {}
    template = loader.get_template('backendPart/gameGallery.html')
    return HttpResponse(template.render(context, request))

def gamePickItUp(request):
    context = {}
    template = loader.get_template('backendPart/games/gamePickItUp.html')
    return HttpResponse(template.render(context, request))

def game1(request):
    context = {}
    template = loader.get_template('backendPart/games/game1.html')
    return HttpResponse(template.render(context, request))

def game2(request):
    context = {}
    template = loader.get_template('backendPart/games/game2.html')
    return HttpResponse(template.render(context, request))

@csrf_exempt
def startGame(request):
    if request.method == 'POST':
        form = forms.StartGameForm(request.POST)
        if form.is_valid():
            gameName = form.cleaned_data['gameName']
            # start game with name = gameName