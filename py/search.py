import keys
import json
import string
import urllib.request
import random
import requests
import shutil
import os

# api key is stored in keys.py, should be in .env maybe
API_KEY = keys.API_KEY

def search(query, max_results):
    query = query.translate(str.maketrans('', '', string.punctuation))
    urlData = 'https://youtube.googleapis.com/youtube/v3/search?part=snippet&type=video&maxResults=' + str(max_results) + '&q=' + query + '&key=' + API_KEY
    webURL = urllib.request.urlopen(urlData)
    data = webURL.read()
    encoding = webURL.info().get_content_charset('utf-8')

    results = json.loads(data.decode(encoding))

    # download thumbnails
    for result in results['items']:
        thumbnail_url = result['snippet']['thumbnails']['default']['url']
        r = requests.get(thumbnail_url, stream=True)
        if r.status_code == 200:
            r.raw.decode_content = True
            # make directory if it doesn't exist
            if not os.path.exists(os.getcwd() + '/py/thumbnails/search'):
                os.makedirs(os.getcwd() + '/py/thumbnails/search')
            with open(os.getcwd() + '/py/thumbnails/search/' + result['id']['videoId'] + '.jpg', 'wb') as f:
                shutil.copyfileobj(r.raw, f)
        else:
            print('error downloading thumbnail')
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

    # download thumbnails
    for result in results['items']:
        thumbnail_url = result['snippet']['thumbnails']['default']['url']
        r = requests.get(thumbnail_url, stream=True)
        if r.status_code == 200:
            r.raw.decode_content = True
            # make directory if it doesn't exist
            if not os.path.exists(os.getcwd() + '/py/thumbnails/rand_video'):
                os.makedirs(os.getcwd() + '/py/thumbnails/rand_video')
            with open(os.getcwd() + '/py/thumbnails/rand_video/' + result['id']['videoId'] + '.jpg', 'wb') as f:
                shutil.copyfileobj(r.raw, f)
        else:
            print('error downloading thumbnail')
    return results

def most_popular_videos(max_results):
    urlData = 'https://youtube.googleapis.com/youtube/v3/videos?part=snippet&chart=mostPopular&regionCode=US&maxResults=' + str(max_results) + '&key=' + API_KEY
    webURL = urllib.request.urlopen(urlData)
    data = webURL.read()
    encoding = webURL.info().get_content_charset('utf-8')

    results = json.loads(data.decode(encoding))
    
    resultStr = ""

    # download thumbnails
    for result in results['items']:
        thumbnail_url = result['snippet']['thumbnails']['default']['url']
        r = requests.get(thumbnail_url, stream=True)
        if r.status_code == 200:
            r.raw.decode_content = True
            # make directory if it doesn't exist
            if not os.path.exists(os.getcwd() + '/py/thumbnails/most_popular'):
                os.makedirs(os.getcwd() + '/py/thumbnails/most_popular')
            with open(os.getcwd() + '/py/thumbnails/most_popular/' + result['id'] + '.jpg', 'wb') as f:
                shutil.copyfileobj(r.raw, f)
        else:
            print('error downloading thumbnail')
            
        resultStr += result['id'] + '\n'
        resultStr += result['snippet']['title'] + '\n'
        resultStr += result['snippet']['channelTitle'] + '\n'            
            
    return resultStr

def get_comments(video_id, max_results):
    urlData = 'https://youtube.googleapis.com/youtube/v3/commentThreads?part=snippet&order=relevance&videoId=' + video_id + '&maxResults=' + str(max_results) + '&key=' + API_KEY
    webURL = urllib.request.urlopen(urlData)
    data = webURL.read()
    encoding = webURL.info().get_content_charset('utf-8')

    results = json.loads(data.decode(encoding))
    return results
