from django.shortcuts import render
import json
# Create your views here.
from django.http import HttpResponse
from django.template import loader


def index(request):
    developersList = ["Oleg", "Sonia", "Misha", "Katya", "Oganes", "Julia", "Gribnik"]
    template = loader.get_template('backendPart/index.html')
    context = {
        'developersList': developersList,
    }
    return HttpResponse(template.render(context, request))


def getGameMapJson(request):
    filePath = 'backendPart/static/backendPart/'
    fileName = 'game_map .json'
    file = open(filePath + fileName)
    data = json.load(file)
    return HttpResponse(json.dumps(data), content_type='application/json')


def getObjectsJson(request):
    filePath = 'backendPart/static/backendPart/'
    fileName = 'objects.json'
    file = open(filePath + fileName)
    data = json.load(file)
    return HttpResponse(json.dumps(data), content_type='application/json')
