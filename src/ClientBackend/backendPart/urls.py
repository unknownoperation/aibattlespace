from django.urls import path
from . import views
urlpatterns = [
    path('', views.index, name='index'),
    path('game_map .json', views.getGameMapJson),
    path('objects.json', views.getObjectsJson)
]