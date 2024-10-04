# atomata-and-regular-expressions


Regular expression:
$ - sign multiplication
^ - sign plus Klini
! - sign lack of transition(is for an empty machine)
Multiplication marks must be placed wherever they are intended (they must not be omitted)

main functions and classes:
AutomaticMachine - NKA(create to reg expr or graph) have method for create DKA(RemoveEps())
NormolizedAutomaticMachine - DKA(create to method in AutomaticMachine) have method for create PDKA(DeleteTraslationWithMoreOneWord() and AlgoTomsan), Supplement PDKA(SupplementPDKA()), MIN PDKA(BuildMinPDKAByPDKA()) and reg expr(CreateRegularExpByDKA()), also have debugging method DfsOnString
