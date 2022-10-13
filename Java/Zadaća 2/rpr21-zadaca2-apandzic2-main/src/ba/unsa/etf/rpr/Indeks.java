package ba.unsa.etf.rpr;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

public class Indeks {
    private final int brojIndeksa;
    private final Fakultet fakultet;
    private final Studij studij;
    private final String smjer;
    private Map<Semestar, List<Predmet>> upisaniSemestri;
    private Map<Predmet,Integer> polozeniPredmeti;

    public Indeks(int brojIndeksa, Fakultet fakultet, Studij studij, String smjer) {
        if(fakultet==null) throw new IllegalArgumentException("Fakultet nije ispravan!");
        if(studij==null) throw new IllegalArgumentException("Studij nije ispravan!");
        this.brojIndeksa = brojIndeksa;
        this.fakultet = fakultet;
        this.studij = studij;
        this.smjer = smjer;
        upisaniSemestri=new HashMap<>();
        polozeniPredmeti=new HashMap<>();
    }

    public int getBrojIndeksa() {
        return brojIndeksa;
    }

    public Fakultet getFakultet() {
        return fakultet;
    }

    public Studij getStudij() {
        return studij;
    }

    public String getSmjer() {
        return smjer;
    }

    public Map<Semestar, List<Predmet>> getUpisaniSemestri() {
        return upisaniSemestri;
    }

    public boolean upisiSemestar(Semestar semestar, List<Predmet> izborniPredmeti) {
        if(semestar==null) throw new IllegalArgumentException("Semestar nije ispravan!");
        int ects=0;
        for(Predmet predmet: izborniPredmeti) {
            if(!semestar.getIzborniPredmeti().contains(predmet)) throw new IllegalArgumentException("Izborni predmet ne pripada ovom semestru!");
            ects=ects+predmet.getEcts();
        }
        if(semestar.getEctsObavezni() + ects != 30) return false;
        upisaniSemestri.put(semestar,izborniPredmeti);
        return true;
    }

    public Map<Predmet,Integer> getPolozeniPredmeti() {
        return polozeniPredmeti;
    }

    public void predmetPolozen(Predmet predmet, Integer ocjena) {
        if(predmet==null) throw new IllegalArgumentException("Predmet je neispravan!");
        if(ocjena<6 || ocjena>10) throw new IllegalArgumentException("Ocjena neispravna!");
        polozeniPredmeti.put(predmet,ocjena);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Indeks indeks = (Indeks) o;
        return brojIndeksa == indeks.brojIndeksa && fakultet.equals(indeks.fakultet) && studij.equals(indeks.studij) && smjer.equals(indeks.smjer) && Objects.equals(upisaniSemestri, indeks.upisaniSemestri) && Objects.equals(polozeniPredmeti, indeks.polozeniPredmeti);
    }

    @Override
    protected Object clone() throws CloneNotSupportedException {
        return super.clone();
    }

    @Override
    public int hashCode() {
        return Objects.hash(brojIndeksa);
    }

    public Map<Predmet, Integer> dajOcjene() {
        return polozeniPredmeti;
    }

}
