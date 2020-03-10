# This is a _very simple_ example of a web service that recognizes faces in uploaded images.
# Upload an image file and it will check if the image contains a picture of Daniel.
# The result is returned as json. For example:
#
# $ curl -XPOST -F "file=@Daniel2.jpg" http://192.168.1.85:5001
#
# Returns:
#
# {
#  "face_found_in_image": true,
#  "is_picture_of_Daniel": true
# }
#
# This example is based on the Flask file upload example: http://flask.pocoo.org/docs/0.12/patterns/fileuploads/
# I have retrained the network on my own face, and added automatic rotation based on EXIF data.
from flask import Flask, request, redirect
import webDetectLib

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def upload_image():

    # Check if a valid image file was uploaded
    if request.method == 'POST':
        if 'file' not in request.files:
            return redirect(request.url)

        file = request.files['file']

        if file.filename == '':
            return redirect(request.url)

        if file and webDetectLib.valid_file(file.filename):
            # The image filename is valid, now deal with any
            # potential orientation.
            file.save('original.jpg')
            webDetectLib.normalize_image('original.jpg')
            return webDetectLib.detect_faces_in_image('oriented.jpg')

    # If no valid image file was uploaded, show the file upload form:
    return '''
    <!doctype html>
    <title>Is this a picture of Daniel?</title>
    <h1>Upload a picture and see if it's a picture of Daniel!</h1>
    <form method="POST" enctype="multipart/form-data">
      <input type="file" name="file">
      <input type="submit" value="Upload">
    </form>
    '''

if __name__ == "__main__":
    app.run(host='192.168.1.85', port=5001, debug=True)