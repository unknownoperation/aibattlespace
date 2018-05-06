from django import forms

class logInForm(forms.Form):
    username = forms.CharField
    password = forms.PasswordInput
