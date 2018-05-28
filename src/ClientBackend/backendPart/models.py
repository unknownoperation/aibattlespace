from django.db import models
from django.contrib.auth.models import User
## Model of user's profile
class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.DO_NOTHING)
    aiFolderPath = models.URLField()
