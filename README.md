# The Light Corridor

&nbsp;

![The Light Corridor](https://www.guilhemduval.fr/images/thumbnail_lightcorridor.jpg)

&nbsp; 

*The Light Corridor* est une réinterprétation du jeu classique *The Light Corridor* sorti sur Atari ST et Amiga en 1990. Ce projet est une implémentation technique entièrement développée en OpenGL.

Le jeu repose sur un moteur graphique conçu sur mesure, sans aucun shader, incluant un système de lumière entièrement développés. Plusieurs thèmes graphiques sont disponibles pour offrir une expérience visuelle variée.

&nbsp; 

## 🛠 Technologies utilisées

- **Langage** : C, C++
- **Bibliothèques** : OpenGL, GLFW, GLM
- **Moteur graphique** : Développement maison en OpenGL, sans shader
- **Système de lumière** : Implémentation propriétaire sans shader

&nbsp; 
  
## 📷 Aperçu

![Aperçu de The Light Corridor](https://www.guilhemduval.fr/images/single_lightcorridor.jpeg)

&nbsp; 

## 🚀 Installation et exécution

### 🖥 Prérequis

- **Système d'exploitation** : Windows, macOS ou Linux
- **Compilateur** : GCC, Clang ou MSVC
- **Outils de construction** : [CMake](https://cmake.org/) installé
- **Bibliothèques** : [GLFW](https://www.glfw.org/) incluse dans le dossier `third_party/`

### 📥 Cloner le dépôt

```bash
git clone https://github.com/L0R3Z/The-IMAC-Light-Corridor.git
cd The-IMAC-Light-Corridor
```

### 🏗 Compilation

1. **Installer CMake** : Téléchargez et installez [CMake](https://cmake.org/download/).
2. **Utilisation avec VS Code** :
   - Il est recommandé d'utiliser l'extension `ms-vscode.cmake-tools`.
   - Configurez l'extension avec votre compilateur. Voir [ce tutoriel](https://cmake.org/cmake/help/latest/guide/tutorial/) pour plus de détails.

3. **Compilation manuelle avec CMake** :
   - Créez un dossier de compilation et exécutez CMake :

     ```bash
     mkdir build && cd build
     cmake ..
     ```

   - CMake détectera automatiquement le compilateur et générera les fichiers appropriés (Makefile, Visual Studio solution, Xcode project, etc.).

4. **Compiler le projet** :
   - **Sur Linux/macOS** :
     ```bash
     make
     ```
   - **Sur Windows (Visual Studio)** :
     Ouvrez `LightCorridor.sln` et compilez le projet.

5. **Exécutable généré** :
   - L'exécutable se trouvera dans `build` ou un sous-dossier selon votre système.

### 🎮 Lancer le jeu

- **Linux/macOS** :
  ```bash
  ./LightCorridor
  ```
- **Windows** :
  Exécutez `LightCorridor.exe` depuis l'Explorateur de fichiers ou via l'invite de commandes.
