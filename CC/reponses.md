---
lang      : fr
title     : Contrôle de connaissances SE207 "SystemC"
date      : 28 juin 2017
---

# Instructions

Ce contrôle de connaissances est strictement individuel.
Vous devez modifier ce fichier pour y inclure vos réponses puis l'ajouter à votre dépôt dans un dossier `CC` à la racine de ce dernier.

Seules les parties entre *`---`* sont à modifier.

# Questions

### Question 1

En SystemC, pour modéliser des calculs sur des entiers signés, plusieurs types peuvent être utilisés.
Donnez la liste de ces types (les grandes familles) en expliquant dans quels cas il est préférable d'utiliser l'un plutôt que l'autre.

---

On a les types suivants :
- `int32_t` Utile lorsque l'on veut simplement effectuer des opération arithmétiques sans action individuelle sur les bits.
- `sc_int<32>` Utile pour effectuer des calculs arithmétiques en plus d'opérations de séléction sur les bits.
- `sc_bv<32>` Utile pour effectuer des opération sur les bits mais on ne peut pas effectuer de calculs arithmétiques.
- `sc_lv<32>` De même que sc_bv, le fait que ce soit des valeurs logique rajoute l'état indéfini et l'état haute impédance (X et Z).

---

### Question 2

Pourquoi peut-on connecter *directement* la sortie (`sc_out<>`) d'un module à la sortie d'un autre module mais pas à une entrée (`sc_in<>`)?

---

Les `sc_out<>` et les `sc_in<>` sont des coquilles vides qui ne font que transmettre les appels à read ou à write au signal connecté.
Ainsi quand on modifie la valeur de la sortie du premier module, la méthode write sera appelée. Cet appel sera propagé jusqu'a la sortie du deuxième module. Par la suite,
l'appel sera encore propagé jusqu'au signal connecté à la sortie du sc_out du deuxième module.

Si l'on connecte un `sc_out<>` à un `sc_in<>`, le sc_in<> ne pourra alors pas être connecté à un signal. Par conséquent la propagation sera arrétée (Normalement ce cas n'est
même pas possible à observer puisqu'il est nécéssaire de connecter tout port à un signal).

---

### Question 3

- Que se passe-t-il si une boucle infinie existe dans une `SC_METHOD`?
- Que se passe-t-il si la fonction `wait()` est appelée dans cette boucle infinie?


---
Si il y a une boucle infinie dans une `SC_METHOD` alors, le scheduler ne reprendra jamais la main puisque la méthode ne terminera pas. Ainsi la simulation
sera bloquée.
Si on rajoute `wait()`, le résultat sera le même puisque l'appel à wait ne rendra pas le contrôle au scheduler puisque le wait va provoquer l'attente d'un évenement de la liste
de sensibilité de la `SC_METHOD` ainsi comme le scheduler ne controle plus la simulation, aucun évenement ne pourra être notifié et donc la simulation sera bloquée .
---

### Question 4

Nous voulons modéliser un bloc matériel synchrone (à une horloge `clk`) dans lequel une étape de traitement doit attendre la fin d'une autre étape avant de commencer.

Pour ce fait, nous utilisons deux `SC_THREAD`, sensibles au front montant de l'horloge, pour modéliser chaque étape.

Dans une première implémentation, nous utilisons un `sc_mutex` pour synchroniser les de `SC_THREAD`.

```{.cpp}
    // Thread 1
    // étape 1 du traitement
    step1_end_mutex.lock();
    …
    step1_end_mutex.unlock();
    wait();


    // Thread 2
    wait();
    // attente de la fin de l'étape 1
    step1_end_mutex.lock();
    // passage à l'étape suivante
    …
```

Dans une seconde implémentation nous utilisons un `sc_signal` dont nous examinons la valeur à chaque front de l'horloge.

```{.cpp}
    // Thread 1
    // étape 1 du traitement
    end_step1 = false;
    …
    end_step1 = true;
    wait();

    // Thread 2
    wait();
    // attente de la fin de l'étape 1
    while(!end_step1) wait();
    // passage à l'étape suivante
    …
```

- Expliquez brièvement le fonctionnement.
- En justifiant votre réponse:
    * Voyez-vous des différences en termes de ressources utilisées pour la simulation (temps de calcul, mémoire…)?
    * Voyez-vous des différences quant à la précision temporelle des deux implémentations?

---
Dans la première implémentation, le thread 1 verouille le mutex en premier puisque le second thread commence par un wait.
Par la suite, le thread 2 se retrouve bloqué sur le lock du mutex. Pendant ce bloquage, le premier thread effectue l'étape 1 du traitement.
Une fois l'étape 1 terminée, le mutex est liberé. Ainsi, le second thread se débloque et effectue le seconde étape du traitement.

Dans la seconde implémentation, le thread 2 commence par un wait. Ainsi, le premier thread met end_step1 à false en premier. Par la suite,
la première étape du traitement est effectuée. Pendant ce temps, le thread 2 boucle en attendant que end_step1 passe a true. Une fois que le traitement
du thread 1 est terminé, end_step1 passe à true. À ce moment là, le second thread sa débloqué au prochain front montant.

La deuxième implémentation utilisera plus de ressources puisqu'on utilise un `sc_signal` pour créer un mutex alors que les mutex sont déja implémentés et
optimisés par SystemC.

Il n'y a pas de différences quand à la précision temporelle puisque l'appel de lock utilise en fait des wait() ainsi les deux implémentation sont équivalentes
au niveau de la prédiction temporelle.

---


### Question 5

- Pouvons-nous modéliser au niveau RTL en utilisant un `SC_THREAD`?
- *Justifiez votre réponse*


---

Le `SC_THREAD` représente un autre fil d'éxécution, par conséquent il est difficile de modéliser au niveau RTL, même si l'on peut se rapprocher d'une modélisation RTL en plaçant des `wait()`` pour simuler un module synchrone.

---
