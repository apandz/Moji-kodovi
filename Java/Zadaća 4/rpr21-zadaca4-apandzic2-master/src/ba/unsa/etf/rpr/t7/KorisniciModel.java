package ba.unsa.etf.rpr.t7;

import javafx.beans.property.SimpleObjectProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;
import java.sql.*;
import java.util.Scanner;

public class KorisniciModel {
    private ObservableList<Korisnik> korisnici = FXCollections.observableArrayList();
    private SimpleObjectProperty<Korisnik> trenutniKorisnik = new SimpleObjectProperty<>();

    private static Connection conn;
    private PreparedStatement ps, dodajUpit, idUpit, obrisiUpit;

    public KorisniciModel() {
        String url="jdbc:sqlite:korisnici.db";
        try {
            conn= DriverManager.getConnection(url);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        try {
            ps=conn.prepareStatement("SELECT * FROM korisnik");
            dodajUpit=conn.prepareStatement("INSERT INTO korisnik VALUES (?,?,?,?,?,?)");
            idUpit= conn.prepareStatement("SELECT MAX(id) FROM korisnik");
            obrisiUpit= conn.prepareStatement("DELETE FROM korisnik WHERE id=?");
        }catch(SQLException e) {
            kreirajBazu();
            try {
                ps=conn.prepareStatement("SELECT * FROM korisnik");
                dodajUpit=conn.prepareStatement("INSERT INTO korisnik VALUES (?,?,?,?,?,?)");
                idUpit= conn.prepareStatement("SELECT MAX(id) FROM korisnik");
                obrisiUpit= conn.prepareStatement("DELETE FROM korisnik WHERE id=?");
            } catch (SQLException ex) {
                ex.printStackTrace();
            }
        }
    }

    private void kreirajBazu() {
        Scanner ulaz = null;
        try {
            ulaz = new Scanner(new FileInputStream("korisnik.sql"));
            StringBuilder sqlUpit = new StringBuilder();
            while (ulaz.hasNext()) {
                sqlUpit.append(ulaz.nextLine());
                if ( sqlUpit.length() > 1 && sqlUpit.charAt( sqlUpit.length()-1 ) == ';') {
                    try {
                        Statement stmt = conn.createStatement();
                        stmt.execute(sqlUpit.toString());
                        sqlUpit = new StringBuilder();
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                }
            }
            ulaz.close();
        } catch (FileNotFoundException e) {
            System.out.println("Ne postoji SQL datoteka... nastavljam sa praznom bazom");
        }
    }

    public void napuni() {
        // Ako je lista već bila napunjena, praznimo je
        // Na taj način se metoda napuni() može pozivati više puta u testovima
        korisnici.clear();
        try {
            ResultSet rs=ps.executeQuery();
            while(rs.next()) {
                int id=rs.getInt(1);
                String ime=rs.getString(2);
                String prezime=rs.getString(3);
                String email=rs.getString(4);
                String username=rs.getString(5);
                String password=rs.getString(6);
                korisnici.add(new Korisnik(ime,prezime,email,username,password));
                korisnici.get(korisnici.size()-1).setId(id);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        trenutniKorisnik.set(null);
    }

    public void vratiNaDefault() {
        // Dodali smo metodu vratiNaDefault koja trenutno ne radi ništa, a kada prebacite Model na DAO onda
        // implementirajte ovu metodu
        // Razlog za ovo je da polazni testovi ne bi padali nakon što dodate bazu
        try {
            Statement stmt=conn.createStatement();
            stmt.executeUpdate("DELETE FROM korisnik");
            kreirajBazu();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void diskonektuj() {
        try {
            conn.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public ObservableList<Korisnik> getKorisnici() {
        return korisnici;
    }

    public void setKorisnici(ObservableList<Korisnik> korisnici) {
        this.korisnici = korisnici;
    }

    public Korisnik getTrenutniKorisnik() {
        return trenutniKorisnik.get();
    }

    public SimpleObjectProperty<Korisnik> trenutniKorisnikProperty() {
        return trenutniKorisnik;
    }

    public void setTrenutniKorisnik(Korisnik trenutniKorisnik) {
        update();
        this.trenutniKorisnik.set(trenutniKorisnik);
    }

    public void setTrenutniKorisnik(int i) {
        update();
        this.trenutniKorisnik.set(korisnici.get(i));
    }

    public void update() {
        if(trenutniKorisnik.getValue()!=null) {
            try {
                PreparedStatement ps1 = conn.prepareStatement("SELECT * FROM korisnik WHERE id=?");
                int trenutniKorisnikID=trenutniKorisnik.getValue().getId();
                ps1.setInt(1, trenutniKorisnikID);
                ResultSet rs1 = ps1.executeQuery();
                String newIme=trenutniKorisnik.getValue().getIme();
                String newPrezime=trenutniKorisnik.getValue().getPrezime();
                String newEmail=trenutniKorisnik.getValue().getEmail();
                String newUsername=trenutniKorisnik.getValue().getUsername();
                String newPassword=trenutniKorisnik.getValue().getPassword();
                if(rs1.next()){
                    if(!rs1.getString(2).equals(newIme)) {
                        try {
                            PreparedStatement promijeniIme= conn.prepareStatement("UPDATE korisnik SET ime=? WHERE id=?");
                            promijeniIme.setString(1,newIme);
                            promijeniIme.setInt(2,trenutniKorisnikID);
                            promijeniIme.executeUpdate();
                            promijeniIme.close();
                        } catch (SQLException e) {
                            e.printStackTrace();
                        }
                    }
                if(!rs1.getString(3).equals(newPrezime)) {
                    try {
                        PreparedStatement promijeniPrezime= conn.prepareStatement("UPDATE korisnik SET prezime=? WHERE id=?");
                        promijeniPrezime.setString(1,newPrezime);
                        promijeniPrezime.setInt(2,trenutniKorisnikID);
                        promijeniPrezime.executeUpdate();
                        promijeniPrezime.close();
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                }
                if(!rs1.getString(4).equals(newEmail)) {
                    try {
                        PreparedStatement promijeniEmail= conn.prepareStatement("UPDATE korisnik SET email=? WHERE id=?");
                        promijeniEmail.setString(1,newEmail);
                        promijeniEmail.setInt(2,trenutniKorisnikID);
                        promijeniEmail.executeUpdate();
                        promijeniEmail.close();
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                }
                if(!rs1.getString(5).equals(newUsername)) {
                    try {
                        PreparedStatement promijeniUsername= conn.prepareStatement("UPDATE korisnik SET username=? WHERE id=?");
                        promijeniUsername.setString(1,newUsername);
                        promijeniUsername.setInt(2,trenutniKorisnikID);
                        promijeniUsername.executeUpdate();
                        promijeniUsername.close();
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                }
                if(!rs1.getString(6).equals(newPassword)) {
                    try {
                        PreparedStatement promijeniPassword= conn.prepareStatement("UPDATE korisnik SET password=? WHERE id=?");
                        promijeniPassword.setString(1,newPassword);
                        promijeniPassword.setInt(2,trenutniKorisnikID);
                        promijeniPassword.executeUpdate();
                        promijeniPassword.close();
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                }
                }
                ps1.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    public int noviId() {
        try {
            ResultSet rs=idUpit.executeQuery();
            if(rs.next()) return rs.getInt(1)+1;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return 1;
    }

    public void dodaj(int id) {
        try {
            dodajUpit.setInt(1,id);
            dodajUpit.setString(2,"");
            dodajUpit.setString(3,"");
            dodajUpit.setString(4,"");
            dodajUpit.setString(5,"");
            dodajUpit.setString(6,"");
            dodajUpit.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void obrisi() {
        if(trenutniKorisnik.isNotNull().get()){
            try {
                obrisiUpit.setInt(1,trenutniKorisnik.getValue().getId());
                obrisiUpit.executeUpdate();
                obrisiUpit.close();
                korisnici.remove(trenutniKorisnik.getValue());
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    public void zapisiDatoteku(File file) {
        if(file!=null) {
            StringBuilder stringBuilder = new StringBuilder("");
            for (Korisnik korisnik : korisnici) {
                int id = korisnik.getId();
                stringBuilder.append(korisnik.getUsername()).append(":").append(korisnik.getPassword()).append(":").append(id).append(":").append(id).append(":").append(korisnik.getIme()).append(" ").append(korisnik.getPrezime()).append("::\n");
            }
            try {
                if(!file.exists()) {
                    file.createNewFile();
                }
                Files.writeString(file.toPath(), stringBuilder.toString(), StandardOpenOption.WRITE);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public Connection getConn() {
        return conn;
    }
}
