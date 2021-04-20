import xmlrpc.client
import time
import pyautogui # you might have to first `pip install pyautogui`


def clickItem(path):
    # Query spix to get the location of the item
    bounds = s.getBoundingBox(path)
    # bounds = [x, y, width, height] as list
    center_x = bounds[0] + bounds[2] / 2
    center_y = bounds[1] + bounds[3] / 2

    # use pyautogui to move the cursor...
    pyautogui.moveTo(center_x, center_y, duration=1)
    # ...and click
    pyautogui.click()



# Connect to the rpc server running in the qt app
s = xmlrpc.client.ServerProxy('http://localhost:9000')

# Wait a bit so that the user has time to move the app to the foreground
print("Make sure the qt app is in the foreground... waiting 3s...")
time.sleep(3)

# Click buttons
clickItem("mainWindow/Button_1")
clickItem("mainWindow/Button_2")
clickItem("mainWindow/Button_2")
clickItem("mainWindow/Button_1")
clickItem("mainWindow/Button_2")

# Query the qt app for the text contents of the results box
resultText = s.getStringProperty("mainWindow/results", "text")
print("Result:\n{}".format(resultText))
