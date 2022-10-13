package ba.etf.rma22.projekat.interfaces

interface Progres {
    fun noviProgres(progres: Float): Int {
        if(progres>=0.1F && progres<0.3F) {
            return 20
        } else if(progres>=0.3F && progres<0.5F) {
            return 40
        } else if(progres>=0.5F && progres<0.7F) {
            return 60
        } else if(progres>=0.7F && progres<0.9F) {
            return 80
        } else if(progres>=0.9F) {
            return 100
        }
        return 0
    }
}