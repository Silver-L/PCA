# PCA(principal component analysis) for raw file
This is a simple implementation of PCA in Python

## pca_python
#### Requirments
```
1. SimpleITK
2. scikit-learn
```

#### Run PCA
```
python --EUDT_txt [] --outdir [] --image_size []
```


## Evaluation
Evaluation of LSDM(Level Set Distribution Model)

#### Requirments
```
1. Eigen
2. ITK
```

#### How to install ITK
```
1. Using Visual Studio to open folder
2. solution -> Manage NuGet Packages -> Browse
3. Search ITK (produced by Atsushi Saito)
4. Install ITK
```

#### Build .exe
```
1. Generalization: Build ./evaluation/generalization/main.cpp
2. Specificity: Build ./evaluation/specificity/main.cpp
```

#### Run .exe
Use the batch file in ./bat to evaluate the performance of LSDM