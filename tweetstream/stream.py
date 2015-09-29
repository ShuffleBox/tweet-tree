from __future__ import absolute_import, print_function
from tweepy import Stream
from tweepy import OAuthHandler
from tweepy.streaming import StreamListener

import requests
from random import randint
#import Adafruit_CharLCD as LCD

from settings import *
import ipdb

#long,lat SW corner first
SEARCHBOX = [ -97.192791, 33.171211, -97.084947, 33.26176 ]
TWEETTREE_URL = "http://10.0.0.3/tweettree/"
#lcd = LCD.Adafruit_CharLCDPlate()

class listener(StreamListener):

    def on_status(self, status):
        print(status.user.name)
        print(status.text)
        
        #ipdb.set_trace()
        colors = {'group':str(randint(000, 005)).zfill(3),
                  'red': str(randint(000, 254)).zfill(3),
                  'green': str(randint(000, 254)).zfill(3),
                  'blue':str(randint(000, 254)).zfill(3),
                  }
        #ipdb.set_trace()
        request = requests.get(TWEETTREE_URL, timeout=3, params=colors)
        print("\n")
        #print(request.url)
        #print("\n")
        #lcd.message(unicode(status.user.name) + '\n' + unicode(status.text))

    def on_error(self, status):
        print(status)


def main():

    # Setup Twitter Authentication
    auth = OAuthHandler(CKEY, CSECRET)
    auth.set_access_token(ATOKEN, ASECRET)
    #ipdb.set_trace()
    # Initialize the stream
    twitterStream = Stream(auth, listener())
    #twitterStream.filter(track=["UNT", "GMG", "clubwillis"])
    twitterStream.filter(locations=[ -97.192791, 33.171211, -97.084947, 33.26176 ])


if __name__ == "__main__":
    main()