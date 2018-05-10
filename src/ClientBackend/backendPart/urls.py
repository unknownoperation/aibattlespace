from django.urls import path
from . import views
urlpatterns = [
    path('', views.index, name='index'),
    path('game_map .json', views.getGameMapJson),
    path('objects.json', views.getObjectsJson),
    path('logIn', views.logIn),
    path('registration', views.registrationPage),
    #path('index', views.index),
    path('registerUser', views.registerUser),
    path('logOut', views.logOut),
    path('uploadFile', views.uploadFile),
    path('watchMatch', views.watchMatch)
]