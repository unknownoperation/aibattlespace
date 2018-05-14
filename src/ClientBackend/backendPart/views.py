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

    if request.user.is_authenticated:
        print("logged")
        context = {
            'username': request.user.username,
            'userList': userList,
            'logStatus': True,
            'result': "",
        }
        # context.update([('l ogStatus', "Authenticated")])
    else:
        print("logged out")
        context = {
            'userList': userList,
            'logStatus': False,
            'result': "",
        }
        # context.update([('logStatus', "Not authenticated")])

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
                print("Unsuccessful registration: such user exist! Please choose other username.")
                context = {
                    'logStatus': False,
                    'result': "Unsuccessful registration: such user exist! Please choose other username.",
                }
            else:
                newUser = User.objects.create_user(username, email, password)
                newUser.save()

                #person = models.Profile()
                #person.user = newUser
                #person.AiFolderPath = None
                #person.save()

                print("\nSign up was successful!")
                context = {
                    'username': request.user.username,
                    'logStatus': False,
                    'result': "\nSign up was successful!",
                }
        else:
            print("Input data is not valid")
            context = {
                'logStatus': False,
                'result': "Input data is not valid",
            }
    else:
        print("request is not POST")
        context = {
            'username': request.user.username,
            'logStatus': False,
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
                    'username': request.user.username,
                    'logStatus': True,
                    'result': "",
                }
            else:
                print("Username or password is not valid")
                context = {
                    'username': request.user.username,
                    'logStatus': False,
                    'result': "Username or password is not valid",
                }
        else:
            print("Input data is not valid")
            context = {
                'username': request.user.username,
                'logStatus': False,
                'result': "Username or password is not valid",
            }
    else:
        print("request is not POST")
        context = {
            'username': request.user.username,
            'logStatus': True,
            'result': "",
        }
    return render(request, 'backendPart/index.html', context)

@csrf_exempt
def logOut(request):
    print("log out view started")
    logout(request)
    context = {
        'logStatus': False,
        'result': "",
    }
    return render(request, 'backendPart/index.html', context)


@csrf_exempt
def uploadFile(request):
    username = request.user.username

    # for p in models.Profile.objects.raw('SELECT AiFolderPath from Profile'):
    #   print(p)

    user = User.objects.get(username=username)
    path = ""

    file = request.FILES['AIsource']
    playerName = str(request.FILES['AIsource'].name)

    # Write sent file
    with open(str(path) + 'AI_' + playerName + ".cpp", 'wb+') as destination:
        for chunk in file.chunks():
            destination.write(chunk)
    return HttpResponse("file was uploaded")


def watchMatch(request):
    context = {}
    template = loader.get_template('backendPart/PickItUpWatch.html')
    return HttpResponse(template.render(context, request))
