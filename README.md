# Blockchain

## v0.1

Blockcainas realizuotas naudojantis mano hash funkcija.

### Sugeneruota yra 1000 varotojų.
Jie turi:
- `Vardą`
- `public_key`
- `Balansą` nuo 100 iki 1000000
### Sugeneruota yra 10000 transakcijų.
- `Siuntėjo indeksą`
- `Gavėjo indeksą`
- `Suma siuntimo` nuo 100 iki 1000000
- `Time stamp` data nusiuntimo
- `Time stamp` sudėjus visus šiuos duomenis bei gavejo public key
### Sugeneruota yra 10000 transakcijų.
- `Siuntėjo indeksą`
- `Gavėjo indeksą`
- `Suma siuntimo` nuo 100 iki 1000000
-  `Time stamp` data nusiuntimo
-  `Time stamp` sudėjus visus šiuos duomenis bei gavejo public key
### Blockas susidaro
- `Prev_Bock_Hash`
- `timestamp` iškasimo data
- `Version` naudojamas v0.2
- `Merkel_Root` tranzakcijų hash
- `Nonce`
- `difficulty_target`
-  `Body_Transactions` visos tranzakcijos
-  `Current_Hash`

Blokas iškasamas, kai keliant `Nonce` vienetu hash gaunamas su tiek nulių pradžioje kiek nusako `difficulty_target`.

Hashinama yra `Prev_Bock_Hash`, `Nonce`, `Version`, `timestamp` ir `Merkel_Root`.

Kai blokas pridedamas prie grandinės atnaujinamos vartotojų sąskaitos bei ištrinamos blocko tranzakcijos iš viso pool.

## Naudojimas

![alt text](https://i.imgur.com/GR9b0s0.png)

Galima pasirinkti tarp v0.1 bei v0.2+v0.3

Toliau galima pasirinkti difficulty_target

Su 10000 tranzakcijų gaunama virš 40 hashų.

![alt text](https://i.imgur.com/ITipxWH.png)



## v0.2+v0.3
Atrenkat 100 tranzakcijų patikrinamas balansas bei hashas, jei randama klaida tranzakcija ištrinama iš poolo.

Pridėti penki 1A, 1B, 1C, 1D, 1E kandidatai, jei neranda su n=100000 kartų pereinanant per visus kandidatus, bandymų kiekis keliamas 2n.


