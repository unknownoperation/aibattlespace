from django import forms

class logInForm(forms.Form):
    username = forms.CharField(max_length=32)
    password = forms.CharField(max_length=32, widget=forms.PasswordInput())
    #password_confirm = forms.CharField(widget=forms.PasswordInput())

class SignUpForm(forms.Form):
    username = forms.CharField(max_length=32)
    password = forms.CharField(max_length=32, widget=forms.PasswordInput())
    email = forms.EmailField(max_length=32, widget=forms.EmailInput())

class UploadFileForm(forms.Form):
    #file = forms.FileField()
    file = forms.FileInput()
    #file = forms.FileInput(widget=forms.FileInput(attrs={'multiple': 'multiple'}))
