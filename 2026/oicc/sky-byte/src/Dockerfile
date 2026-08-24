FROM python:3.12-slim
WORKDIR /app

RUN useradd -m chall
RUN pip install pycryptodome
RUN apt update
RUN apt install -y socat

COPY --chmod=444 sky-byte.py .

USER chall

ENV FLAG="oiccflag{??who_is_the_smartest_shark_in_town??}"
ENTRYPOINT ["socat","TCP-LISTEN:1337,reuseaddr,fork","EXEC:python3 -u sky-byte.py"]
