from django.shortcuts import render

# Create your views here.
from django.http import HttpResponse
from django.template import loader


def index(request):
    developersList = ["Oleg", "Sonia", "Misha", "Katya", "Oganes", "Julia", "Gribnik"]
    template = loader.get_template('testDjango/index.html')
    context = {
        'developersList': developersList,
    }
    return HttpResponse(template.render(context, request))
