from kafka import KafkaProducer
import sys

if __name__ == '__main__':
  if len(sys.argv) != 2:
    print('Usage: kafkaProducer.py <hostname>:<port>', file=sys.stderr)
  
  producer = KafkaProducer(bootstrap_servers=sys.argv[1])

  while True:
    message = input()
    producer.send('pspd-topic', str.encode(message))