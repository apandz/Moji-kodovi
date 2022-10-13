package ba.unsa.etf.rpr;

import java.util.*;

public class Fakultet {
    private String naziv;
    private List<Studij> studiji;
    private List<Student> studenti;
    private List<Profesor> profesori;
    private static int indeks=1;

    public Fakultet(String naziv) {
        if(naziv.isBlank()) throw new IllegalArgumentException("Fakultet mora imati naziv!");
        this.naziv = naziv;
        studiji=new ArrayList<>();
        studenti=new ArrayList<>();
        profesori=new ArrayList<>();
    }

    public String getNaziv() {
        return naziv;
    }

    public void setNaziv(String naziv) {
        if(naziv.isBlank()) throw new IllegalArgumentException("Fakultet mora imati naziv!");
        this.naziv=naziv;
    }

    public int getIndeks() {
        return indeks;
    }

    public List<Studij> getStudiji() {
        return studiji;
    }

    public void dodajStudij(Studij studij) {
        if(studij==null) throw new IllegalArgumentException("Neispravan studij!");
        if(studiji.contains(studij)) throw new IllegalArgumentException("Studij već postoji na fakultetu!");
        studiji.add(studij);
    }

    public void izbaciStudij(Studij studij) {
        if(studij==null) throw new IllegalArgumentException("Neispravan studij!");
        if(!studiji.remove(studij)) throw new IllegalArgumentException("Studij ne postoji na fakultetu!");
    }

    public List<Student> getStudenti() {
        return studenti;
    }

    public void upisiStudenta(Student student) {
        if(student==null) throw new IllegalArgumentException("Neispravan student!");
        if(studenti.contains(student)) throw new IllegalArgumentException("Student je već na ovom fakultetu!");
        studenti.add(student);
        indeks++;
    }

    public void ispisiStudenta(Student student) {
        if(student==null) throw new IllegalArgumentException("Neispravan student!");
        if(!studenti.remove(student)) throw new IllegalArgumentException("Student nije na ovom fakultetu!");
    }

    public List<Profesor> getProfesori() {
        return profesori;
    }

    public void dodajProfesora(Profesor profesor) {
        if(profesor==null) throw new IllegalArgumentException("Neispravan profesor!");
        if(profesori.contains(profesor)) throw new IllegalArgumentException("Profesor je već na ovom fakultetu!");
        profesori.add(profesor);
    }

    public void izbaciProfesora(Profesor profesor) {
        if(profesor==null) throw new IllegalArgumentException("Neispravan profesor!");
        if(!profesori.remove(profesor)) throw new IllegalArgumentException("Profesor nije na ovom fakultetu!");
    }

    public List<Profesor> dajProfesoreBezNormeIliPrekoNorme () {
        return profesori.stream().filter(profesor -> profesor.getNorma()>150 || profesor.getNorma()<120).toList();
    }

    public List<Profesor> dajSortiraneProfesorePoNormi() {
        return profesori.stream().sorted(Comparator.comparingInt(Profesor::getNorma)).toList();
    }

    public List<Profesor> dajSortiraneProfesorePoBrojuStudenata() {
        return profesori.stream().sorted((profesor1, profesor2)-> {
            int brojStudenata1=0;
            int brojStudenata2=0;
            for(Predmet predmet: profesor1.getPredmeti()) {
                brojStudenata1=brojStudenata1+predmet.getStudenti().size();
            }
            for(Predmet predmet: profesor2.getPredmeti()) {
                brojStudenata2=brojStudenata2+predmet.getStudenti().size();
            }
            return brojStudenata1-brojStudenata2;
        }).toList();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Fakultet fakultet = (Fakultet) o;
        return naziv.equals(fakultet.naziv) && Objects.equals(studiji, fakultet.studiji) && Objects.equals(studenti, fakultet.studenti) && Objects.equals(profesori, fakultet.profesori);
    }

    @Override
    public int hashCode() {
        return Objects.hash(naziv);
    }

}
