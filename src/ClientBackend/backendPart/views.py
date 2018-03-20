import json
import socket
# Create your views here.
from django.http import HttpResponse
from django.shortcuts import render
from django.template import loader


def index(request):
    developersList = ["Oleg", "Sonia", "Misha", "Katya", "Oganes", "Julia", "Gribnik"]
    template = loader.get_template('backendPart/index.html')
    context = {
        'developersList': developersList,
    }
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
        sock = socket.socket()  # Do not know when file closes to know hen to close socket, so now open and close it in every request
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


def getGameMapJson(request):
    fileName = 'game_map .json'
    # Getting JSON with TCP from game server
    blockSize = 1024  # Get JSON partly
    readed, data = readFromSocket(blockSize)
    if not readed:
        data = readDirectlyFromFile(fileName)
    return HttpResponse(data, content_type='application/json')


def getObjectsJson(request):
    fileName = 'objects.json'
    # Getting JSON with TCP from game server
    blockSize = 128  # Get JSON partly
    readed, data = readFromSocket(blockSize)
    if not readed:
        data = readDirectlyFromFile(fileName)
    return HttpResponse(data, content_type='application/json')
