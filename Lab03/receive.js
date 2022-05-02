#!/usr/bin/env node

var amqp = require('amqplib/callback_api');

var smallest = 0;
var biggest = 0;
let result = [];

function analyse_vector(vector) {
  for (let i = 0; i < 100; i++) {
    if (vector[i] < smallest) {
      smallest = vector[i];
    }
    if (vector[i] > biggest) {
      biggest = vector[i];
    }
  }
}

function server() {
  amqp.connect('amqp://localhost', function (error0, connection) {
    if (error0) {
      throw error0;
    }
    connection.createChannel(function (error2, channel1) {
      if (error2) {
        throw error2;
      }

      channel1.assertExchange('queue1', 'fanout', {
        durable: false,
      });

      channel1.assertQueue(
        '',
        {
          exclusive: true,
        },
        function (error2, q) {
          if (error2) {
            throw error2;
          }
          console.log(
            ' [*] Waiting for messages in %s. To exit press CTRL+C',
            q.queue
          );
          channel1.bindQueue(q.queue, 'queue1', '');

          channel1.consume(
            q.queue,
            function (msg) {
              if (msg.content) {
                analyse_vector(msg.content.toString().split(',').map(Number));

                console.log(' [x] Maior %s | Menor %s', biggest, smallest);

                result = [biggest, smallest];

                connection.createChannel(function (error1, channel2) {
                  if (error1) {
                    throw error1;
                  }

                  channel2.assertExchange('queue2', 'fanout', {
                    durable: false,
                  });
                  channel2.publish('queue2', '', Buffer.from(result.toString()));
                  console.log(' [x] Sent %s', result.toString());
                });
              }
            },
            {
              noAck: true,
            }
          );
        }
      );
    });
  });
}

server();