from tweepy import Stream
from tweepy import OAuthHandler
from tweepy.streaming import StreamListener

import Adafruit_CharLCD as LCD

from settings import *
import ipdb

lcd = LCD.Adafruit_CharLCDPlate()

class listener(StreamListener):

    def on_status(self, status):
        print status
        ipdb.set_trace()
        lcd.message(unicode(status.user.name) + '\n' + unicode(status.text))

    def on_error(self, status):
        print status


def main():

    # Setup Twitter Authentication
    auth = OAuthHandler(CKEY, CSECRET)
    auth.set_access_token(ATOKEN, ASECRET)

    # Initialize the stream
    twitterStream = Stream(auth, listener())
    #twitterStream.filter(track=["UNT library", "meangreen", "clubwillis"])
    twitterStream.filter(track=["#apple"])


if __name__ == "__main__":
    main()