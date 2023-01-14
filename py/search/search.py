import keys
import json
import string
import urllib.request

def search(query):
    # api key stored in keys.py - temporary (should be changed to .env or something)
    API_KEY = keys.API_KEY
    max_results = 10
    query = query.translate(str.maketrans('', '', string.punctuation))

    urlData = 'https://youtube.googleapis.com/youtube/v3/search?part=snippet&maxResults=' + str(max_results) + '&q=' + \
        query + '&key=' + API_KEY
    webURL = urllib.request.urlopen(urlData)
    data = webURL.read()
    encoding = webURL.info().get_content_charset('utf-8')

    results = json.loads(data.decode(encoding))
    return results
