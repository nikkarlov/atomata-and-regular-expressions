Karlov Nickolay B05-328

# atomata-and-regular-expressions


Regular expression:

$ - sign multiplication

^ - sign plus Klini

! - sign lack of transition(is for an empty machine)

Multiplication marks must be placed wherever they are intended (they must not be omitted)

main functions and classes:

AutomaticMachine - NKA(create to reg expr or graph) have method for create DKA(RemoveEps())

NormolizedAutomaticMachine - DKA(create to method in AutomaticMachine) have method for create PDKA(DeleteTraslationWithMoreOneWord() and AlgoTomsan), Supplement PDKA(SupplementPDKA()), MIN PDKA(BuildMinPDKAByPDKA()) and reg expr(CreateRegularExpByDKA()), also have debugging method DfsOnString

@ - eps translation in graph

![image_2024-10-08_13-04-08](https://github.com/user-attachments/assets/f6508b7d-a5db-439c-8cd3-230ee6c75511)
