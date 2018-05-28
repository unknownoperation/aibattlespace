from django.urls import path
from . import views
urlpatterns = [
    path('', views.index, name='index'),
    path('game_map .json', views.getGameMapJson),
    path('objects.json', views.getObjectsJson),

    path('index', views.index),
    path('logIn', views.logIn),
    path('logOut', views.logOut),
    path('registration', views.registration),

    path('registerUser', views.registerUser),
    path('uploadFile', views.uploadFile),

    path('startGame', views.startGame),
    path('gameGallery', views.gameGallery),
    path('gamePickItUp', views.gamePickItUp),
    path('gameMakeItYours', views.gameMakeItYours),
    path('gameBattleTanks', views.gameBattleTanks),
    path('getUserGuide', views.getUserGuide)
]