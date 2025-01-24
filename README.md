# c-ddns
The c-ddns application dynamically updates your Cloudflare website's IP address using the Cloudflare API, enabling users to maintain a Dynamic DNS. The software is primarily designed to compile and run on a Raspberry Pi Zero or any Linux-based system.


## Required to run this program
- POSIX (you need a unix like os)
- Make
- gcc
- openssl
- Cloud flare account
- registered domain through cloud flare

## Before you run this program

#### Define your openssl paths
In the Makefile, you need to specify the path of your openssl library. You can find these paths by using:
```
which openssl
```
and 
```
whereis openssl
```

Then update these paths in the makefile. 
Example:

```
# OpenSSL paths for linux and Raspberry Pi
OPENSSL_INCLUDE_PATH=/usr/include/openssl
OPENSSL_LIB_PATH=/usr/lib/x86_64-linux-gnu

```
```
# OpenSSL paths for mac/homebrew
OPENSSL_INCLUDE_PATH=/opt/homebrew/opt/openssl@3/include
OPENSSL_LIB_PATH=/opt/homebrew/opt/openssl@3/lib
```

#### Define your secrets
You need to define your secret in 'secrets.c'. 
Run in your terminal:

```
touch secrets.c
```

You need to define your email, cloudflare api key, domain, zone id, dns record id, and the cloud flare domain. You can refer to cloud flare's website to find these items: [Cloud Flare's Website](https://developers.cloudflare.com/api/resources/dns/subresources/records/methods/list/). Everything should be easily found on your home page of your domain on the console. The only thing that was difficult to find was the dns record id. I found it by changing the ip address of the domain and then viewing the audit log to find the Resource ID which is the dns record id. 

Example:
```
#define EMAIL "youremailhere@example.com"
#define CLOUD_FLARE_API_KEY "YourApiKeyHere"
#define DOMAIN "yourDomain.com"
#define ZONEID "yourZoneId"
#define DNS_RECORD_ID "YourDnsRecordId"
#define CLOUD_FLARE_DOMAIN "api.cloudflare.com"
```

#### Install OpenSSL Development
Install the library with your package manager, I am using apt-get. So an example of this is:
```
sudo apt-get install libssl-dev
```

Once you have added these parameters to the secrets.c file, you should be ready to run the program.


## How to run this program
In the root of this repository directory, run this command in the terminal.
```
make
```

You can also this command to see the list of commands from the help file.
```
make help
```

