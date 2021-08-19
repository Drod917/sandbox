import face_recognition
from PIL import Image
from flask import jsonify

VALID_EXTENSIONS = {'png', 'jpg', 'jpeg', 'gif'}

# Rotate images if they are not uploaded up-right.
def normalize_image(filename):
    img = Image.open(filename)

    if (img.mode != 'RGB'):
        print("Image type: ", img.mode, "Converting to RGB...")
        img = img.convert('RGB')
    print("Loaded image file...")
    
    # Attempt to grab orientation value, rotate accordingly
    img_exif = img._getexif()
    try:
        img_orientation = img_exif[274]
    except:
        print("MSG: No EXIF orientation data found")
        img.save('oriented.jpg')
        return

    if img_orientation == 2:
        img = img.transpose(Image.FLIP_LEFT_RIGHT)
    elif img_orientation == 3:
        img = img.transpose(Image.ROTATE_180)
    elif img_orientation == 4:
        img = img.transpose(Image.FLIP_TOP_BOTTOM)
    elif img_orientation == 5:
        img = img.transpose(Image.FLIP_LEFT_RIGHT).transpose(Image.ROTATE_90)
    elif img_orientation == 6:
        img = img.transpose(Image.ROTATE_270)
    elif img_orientation == 7:
        img = img.transpose(Image.FLIP_TOP_BOTTOM).transpose(Image.ROTATE_90)
    elif img_orientation == 8:
        img = img.transpose(Image.ROTATE_90)

    img.save('oriented.jpg')
    return img

def valid_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in VALID_EXTENSIONS

def detect_faces_in_image(file_stream):
    # Pre-calculated face encoding of Daniel

    #known_face = face_recognition.load_image_file('daniel.jpg')
    #known_face_encoding = face_recognition.face_encodings(known_face)[0]
    #print(known_face_encoding)

    # Pre-computing face encoding of my face
    # (calculated using the commented code above)
    known_face_encoding = [-0.12922379,  0.07092743, -0.00354194,  0.02076598, -0.11090854, -0.040275,
                           -0.02921126, -0.21441154,  0.15976422, -0.07616511,  0.22305927,  0.0241609,
                           -0.15142833, -0.03692883,  0.00556865,  0.04607274, -0.08849067, -0.15515187,
                           -0.00479245, -0.07650134,  0.11193989, -0.0052447,   0.07092138,  0.1484945,
                           -0.23314062, -0.24656186, -0.13222456, -0.1664893,   0.06394726, -0.0951724,
                            0.00488862, -0.01896362, -0.20735809, -0.0218506,   0.04966293,  0.07112196,
                            0.00649983, -0.02437863,  0.23333357,  0.01551994, -0.16939279,  0.02560941,
                            0.0066229,   0.2599299,   0.13813865,  0.0375739,   0.06884928, -0.13681956,
                            0.14399879, -0.17529638,  0.15911047,  0.12409961,  0.09541179,  0.03339978,
                            0.04701398, -0.18418147,  0.02041998,  0.14752324, -0.2502971,   0.17998749,
                            0.11249826, -0.06891127, -0.01340514, -0.02739888,  0.25055909,  0.15648423,
                           -0.1456188,  -0.1144432,   0.06237425, -0.1895286,  -0.033273,    0.14095469,
                           -0.13442105, -0.22240601, -0.27524063,  0.07573131,  0.52274477,  0.161138,
                           -0.18455759,  0.01513411, -0.07186101,  0.00737699,  0.08977808,  0.12156806,
                           -0.06995837,  0.0067942,  -0.08667757,  0.07473856,  0.19490221,  0.04486882,
                           -0.04817835,  0.2671293,  -0.03944821,  0.0566981,   0.03338837,  0.09810277,
                           -0.15937342, -0.05174902, -0.00263627, -0.03897098, -0.05050717, -0.02019492,
                            0.06223822,  0.07597315, -0.18770932,  0.16802804, -0.07345092, -0.02769734,
                           -0.07750312, -0.00644107, -0.08684625, -0.00744817,  0.09936486, -0.32999155,
                            0.22438321,  0.08954722,  0.03582214,  0.18244395,  0.00417885,  0.06935451,
                           -0.00788467, -0.00067052, -0.11486842,  0.00405222,  0.06110171, -0.08997641,
                            0.15131773, -0.01412045]

    # Load the uploaded image file
    img = face_recognition.load_image_file(file_stream)
    # Get face encodings for any faces in the uploaded image
    unknown_face_encodings = face_recognition.face_encodings(img)

    face_found = False
    is_Daniel = False

    if len(unknown_face_encodings) > 0: # at least 1 face was found
        face_found = True
        # See if the first face in the uploaded image matches the known face of Daniel
        match_results = face_recognition.compare_faces(
                                [known_face_encoding],
                                unknown_face_encodings[0]
                            )
        if match_results[0]:
            is_Daniel = True

    # Return the result as json
    result = {
        "face_found_in_image": face_found,
        "is_picture_of_Daniel": is_Daniel
    }
    return jsonify(result)