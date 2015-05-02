class AddSondageIdToQuestions < ActiveRecord::Migration
  def change
    add_column :questions, :sondage_id, :integer
  end
end
