# Synth_ENIB

### Diagrammes UML
* Utiliser [Draw.io](https://apps.diagrams.net) pour visualiser tous les diagrammes


## Code du Projet
**NB:** Chaque partie de l'application est développée dans un Dossier (ici Dossier: `Code`) dans un premier temps.

### Exécution
1. Cloner le repo git
1. Télécharger/cloner la bibliothèque [JUCE](https://github.com/juce-framework/JUCE.git)
1. Installer les bibliothèques nécessaires pour JUCE en exécutant le script `Extras/install_libraries.sh`
1. Lancer le `Projucer` (inclus dans la bibliothèque JUCE: `JUCE/extras/Projuicer`) en copiant le script `Extras/run.sh` dans le dossier et en l'exécutant
1. Ouvrir le fichier projucer du projet (ex: Oscillator_TAP.jucer)
1. Clic-Droit sur le bouton `Linux Makefile` et appuyer sur `Save this exporter` (ou faire `ctrl+s`)
1. Si les dossiers `Build` et `JuceLibraryCode` n'ont pas été créé, relancer le Projucer (`./Projucer`) et recommencer l'opération
1. Vérifier que les dossiers `Build` et `JuceLibraryCode` existent
1. Compiler le projet avec le script `run.sh`
   
### Extras
1. Pour avoir l'`autocomplete` sur vscode, copiez le dossier `.vscode` s'il n'est pas présent.



## Collaborateurs
* Sofiya DEBOIS
* Yassine SAHEB ETTABAA
* Romaric HUBERT
