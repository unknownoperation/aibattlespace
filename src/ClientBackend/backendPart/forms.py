from django import forms

## Log in form
# @param username - user's username
# @param password - user's password
class logInForm(forms.Form):
    username = forms.CharField(max_length=32)
    password = forms.CharField(max_length=32, widget=forms.PasswordInput())
    #password_confirm = forms.CharField(widget=forms.PasswordInput())

## Sign up form
# @param username - user's username
# @param password - user's password
# @param email - user's email
class SignUpForm(forms.Form):
    username = forms.CharField(max_length=32)
    password = forms.CharField(max_length=32, widget=forms.PasswordInput())
    email = forms.EmailField(max_length=32, widget=forms.EmailInput())

## Upload file form
# @param file - user's files
# @param gameName - game for which files are uploading
class UploadFileForm(forms.Form):
    #file = forms.FileField()
    file = forms.FileInput()
    gameName = forms.CharField(max_length=32)

## Start game form
# @param gameName - game which user wants to play
class StartGameForm(forms.Form):
    gameName = forms.CharField(max_length=32)
