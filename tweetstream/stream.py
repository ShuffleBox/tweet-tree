from tweepy import Stream
from tweepy import OAuthHandler
from tweepy.streaming import StreamListener
from settings import *

class listener(StreamListener):
    
    def on_status(self, status):
        print status.id
        
    def on_error(self, status):
        print status
        
auth = OAuthHandler(CKEY, CSECRET)
auth.set_access_token(ATOKEN, ASECRET)
twitterStream = Stream(auth, listener())
#twitterStream.filter(track=["UNT library", "meangreen", "clubwillis"])
twitterStream.filter(track=["#AmericanIdol"])