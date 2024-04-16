# Synth_ENIB

## Organisation

### Diagrammes UML
* Utiliser la branche `main`
* Utiliser [Draw.io](https://apps.diagrams.net) pour ouvrir tous les diagrammes


### Exécution
1. Cloner le repo git
1. Télécharger/cloner la bibliothèque JUCE: `https://github.com/juce-framework/JUCE.git`
1. Lancer le `Projucer` (inclus dans la bibliothèque JUCE: `JUCE/extras/Projuicer`) en copiant le script `run.sh` dans le dossier et en l'exécutant
1. Ouvrir le fichier projucer du projet (ex: Synth_1.jucer)
1. Clic-Droit sur le bouton `Linux Makefile` et appuyer sur `Save this exporter`
1. Noramelent dans le dossier maintenant existe les dossiers: `Build` et `JuceLibraryCode`
1. Compiler le projet avec le script `run.sh`
   

### Test du plugin de Synthétiseur (VST3)
1. Après Avoir Téléchargé la bibliothèque JUCE, Compiler et Excécuter `JUCE/extras/AudioPluginHost`. Cela peut se faire avec cmake ou en copiant le script `run.sh` dans le même dossier.
1. Ouvrir AudioPluginHost et ajouter le plugin (vst3) sur l'application: `Edit the list of available plugins` puis `Options/Scan for new updated VST3 Plugins`
1. Relier le `Midi Input (internal)` aux 2 premières entrées du Plugin puis aux 2 premières sorties audio (à gauche)


### Extras
1. Pour avoir l'`autocomplete` sur vscode, copiez le dossier `.vscode` s'il n'est pas présent.



## Collaborateurs
* Sofiya DEBOIS
* Yassine SAHEB ETTABAA
* Romaric HUBERT
