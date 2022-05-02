let collectionArray = [1.0, 5.0, 14.0, 8.0];

console.log('ARRAY -', collectionArray);

let collectionString = collectionArray.toString();

console.log('STRING -', collectionString);

console.log('STRING TO ARRAY -', collectionString.split(',').map(Number));
