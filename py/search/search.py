import keys
import json
import string
import urllib.request
import random

# api key is stored in keys.py, should be in .env maybe
API_KEY = keys.API_KEY

def search(query):
    max_results = 10
    query = query.translate(str.maketrans('', '', string.punctuation))
    urlData = 'https://youtube.googleapis.com/youtube/v3/search?part=snippet&type=video&maxResults=' + str(max_results) + '&q=' + query + '&key=' + API_KEY
    webURL = urllib.request.urlopen(urlData)
    data = webURL.read()
    encoding = webURL.info().get_content_charset('utf-8')

    results = json.loads(data.decode(encoding))
    return results

def rand_video():
    # this is intended to only return one result, otherwise the multiple results will be the similar videos, i think
    max_results = 1
    randomThing = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(3))
    urlData = 'https://youtube.googleapis.com/youtube/v3/search?part=snippet&type=video&maxResults=' + str(max_results) + '&q=' + randomThing + '&key=' + API_KEY
    webURL = urllib.request.urlopen(urlData)
    data = webURL.read()
    encoding = webURL.info().get_content_charset('utf-8')

    results = json.loads(data.decode(encoding))
    return results

def most_popular_videos():
    max_results = 4
    urlData = 'https://youtube.googleapis.com/youtube/v3/videos?part=snippet&chart=mostPopular&regionCode=US&maxResults=' + str(max_results) + '&key=' + API_KEY
    webURL = urllib.request.urlopen(urlData)
    data = webURL.read()
    encoding = webURL.info().get_content_charset('utf-8')

    results = json.loads(data.decode(encoding))
    return results

def get_comments(video_id):
    max_results = 10
    urlData = 'https://youtube.googleapis.com/youtube/v3/commentThreads?part=snippet&order=relevance&videoId=' + video_id + '&maxResults=' + str(max_results) + '&key=' + API_KEY
    webURL = urllib.request.urlopen(urlData)
    data = webURL.read()
    encoding = webURL.info().get_content_charset('utf-8')

    results = json.loads(data.decode(encoding))
    return results
