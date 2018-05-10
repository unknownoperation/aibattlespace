from django import forms

class logInForm(forms.Form):
    username = forms.CharField(max_length=20)
    password = forms.CharField(max_length=20, widget=forms.PasswordInput())
    #password = forms.CharField(widget=forms.PasswordInput())
    #password_confirm = forms.CharField(widget=forms.PasswordInput())

    #class Meta:
     #   model = User
      #  fields = ('username', 'email', 'password')

class SignUpForm(forms.Form):
    username = forms.CharField(max_length=20)
    password = forms.CharField(max_length=20, widget=forms.PasswordInput)
    email = forms.EmailField(max_length=20)
